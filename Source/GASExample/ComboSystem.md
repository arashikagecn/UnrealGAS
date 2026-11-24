# GAS 连招系统

## 设计不变量

- 一个连招节点就是一个独立 `GameplayAbility`，不是同一个 Ability 里切换多个蒙太奇。
- 每个 Ability 只负责自己的蒙太奇、伤害窗口、位移和特效。
- Ability 由 Gameplay Tag 发现和激活，控制器不保存连段序号。
- 当前段、连段窗口、输入缓冲、受击/死亡阻断全部使用 Gameplay Tag 表达。

## 原生标签

实现位于 `GameplayTags/CPP_AbilityTags.h`。

| 标签 | 用途 |
| --- | --- |
| `CPPAbilityTags.Ability.Combo` | 所有连招技能的公共技能标签 |
| `CPPAbilityTags.Ability.Combo.Stage.1..4` | 技能资产标签，便于查询和调试 |
| `CPPAbilityTags.State.Combo.Active` | 当前有连招技能占用角色 |
| `CPPAbilityTags.State.Combo.Window` | 当前动画允许衔接下一段 |
| `CPPAbilityTags.State.Combo.InputBuffered` | 窗口前的连招输入已缓存 |
| `CPPAbilityTags.State.Combo.Blocked` | 外部系统阻断连招，例如受击、硬直 |
| `CPPAbilityTags.State.Combo.Stage.1..4` | 当前连招段的状态标签 |
| `CPPAbilityTags.Event.Combo.WindowOpen` | 动画通知开启连段窗口 |
| `CPPAbilityTags.Event.Combo.WindowClose` | 动画通知关闭连段窗口 |
| `CPPAbilityTags.Event.Combo.InputBuffered` | ASC 通知当前段收到缓存输入 |

`State.Death` 和 `State.Combo.Blocked` 默认阻止所有连招技能激活。

## 运行流程

1. 控制器按当前输入映射调用 `TryActivateAbilityOrBufferCombo`，传入输入对应的 Ability Tag，例如 `CPPAbilityTags.Ability.Primary`。
2. ASC 通过该 Tag 尝试激活已授予的技能。第一段没有前置段标签，只要求不存在 `State.Combo.Active`。
3. 连招技能进入激活态后添加 `State.Combo.Active` 和自己的 `ComboStageTag`。
4. 播放该技能自己的蒙太奇。
5. 蒙太奇的 `GAS Combo Window` 通知状态开启时，发送 `Event.Combo.WindowOpen`，技能添加 `State.Combo.Window`。
6. 后续段同时要求：
   - `State.Combo.Window`
   - `RequiredPreviousStageTag`
   - 输入 Tag 与技能资产标签匹配
7. 新段激活时立刻消费 `State.Combo.Window`，防止同一次 `TryActivateAbilitiesByTag` 循环中连续激活多个候选技能。
8. 如果输入早于窗口，ASC 写入 `State.Combo.InputBuffered` 并通过 GameplayEvent 携带原始输入 Tag；当前段在窗口开启后使用该 Tag 尝试下一段。
9. 窗口关闭或技能结束时清空输入缓冲；最后一段结束会移除 `State.Combo.Active`。

## 蓝图配置

创建三个 Ability 蓝图，父类均为 `UCPP_GA_ComboStage`。每个蓝图只配置自己的逻辑和资源。

### 第一段

- `Combo Montage`：`Primary_A` 对应蒙太奇。
- `Combo Input Tag`：`CPPAbilityTags.Ability.Primary`。
- `Combo Stage Tag`：`CPPAbilityTags.State.Combo.Stage.1`。
- `Required Previous Stage Tag`：留空。
- Asset Tags：包含 `CPPAbilityTags.Ability.Primary`；父类已提供 `CPPAbilityTags.Ability.Combo`。

### 第二段

- `Combo Montage`：`Primary_B` 对应蒙太奇。
- `Combo Input Tag`：`CPPAbilityTags.Ability.Primary`。
- `Combo Stage Tag`：`CPPAbilityTags.State.Combo.Stage.2`。
- `Required Previous Stage Tag`：`CPPAbilityTags.State.Combo.Stage.1`。
- Asset Tags：包含 `CPPAbilityTags.Ability.Primary`。

### 第三段

- `Combo Montage`：第三击自己的蒙太奇。
- `Combo Input Tag`：`CPPAbilityTags.Ability.Primary`。
- `Combo Stage Tag`：`CPPAbilityTags.State.Combo.Stage.3`。
- `Required Previous Stage Tag`：`CPPAbilityTags.State.Combo.Stage.2`。
- Asset Tags：包含 `CPPAbilityTags.Ability.Primary`。

每个攻击蒙太奇都要在允许输入的区间添加通知状态 `GAS Combo Window`。通知区间建议从攻击判定后开始，到取消后摇之前结束。

三个 Ability 都加入角色的 `StartUpAbilities`。如果仍保留原来的 `GA_Primary`，第一击会同时激活多个拥有 `Ability.Primary` 的技能，应禁用原技能或移除它的 Primary 资产标签。

## 分支连招

同一个前置段可以连接多个分支。给分支使用不同的输入 Tag，例如轻击为 `Ability.Primary`、重击为 `Ability.Secondary`；每个 Ability 的 `Combo Input Tag` 与自身 Asset Tags 保持一致。控制器传入的 Tag 会通过输入缓冲事件保留，因此窗口前按重击也会在窗口开启后尝试重击分支。

保证同一前置段下、同一输入 Tag 只匹配一个可激活分支，否则 `TryActivateAbilitiesByTag` 会尝试所有匹配技能。

## 与受击/死亡系统协作

受击或不可行动效果应执行以下任一策略：

- 添加 `CPPAbilityTags.State.Combo.Blocked`，阻止新段进入。
- 通过 GameplayEffect/Ability 取消带 `CPPAbilityTags.Ability.Combo` 资产标签的技能，并添加 `State.Combo.Blocked`。

死亡流程应添加 `CPPAbilityTags.State.Death`。连招技能基类已将该标签设为激活阻断标签。

## 关键代码

- `GameAbilities/CPP_GA_ComboStage.h/.cpp`：单段技能生命周期、窗口、输入缓冲和标签校验。
- `Notify/CPP_ANS_ComboWindow.h/.cpp`：把蒙太奇窗口转换成 GameplayEvent。
- `GameAbilities/CPP_AbilitySystemComponent.h/.cpp`：Tag 激活失败后的连招输入缓存。
- `GameplayTags/CPP_AbilityTags.h/.cpp`：原生 GameplayTag 定义。
