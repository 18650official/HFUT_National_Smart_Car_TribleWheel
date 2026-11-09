# 2024 全国大学生智能汽车竞赛 · 双车组 B 车

**Award / 奖项：** 2024 年全国大学生智能汽车竞赛安徽省三等奖  
**Group / 组别：** 双车组（B 车）  
**Platform / 车辆平台：** 三轮车，两个后轮独立差速驱动，前部从动支撑轮  
**MCU / 主控：** CH32V307VCT6（RISC-V，256 KB Flash / 64 KB SRAM）  
**Camera / 摄像头：** MT9V034（DVP，188 × 120 灰度）  
**Single-side bridge CV tutorial / 单边桥 CV 算法详解：** <https://snowmiku-home.top/2024/07/18/post-2024718/>

> 这份 README 面向后续参赛同学、想复现本车的人，以及想学习摄像头循迹算法的读者。  
> This README is written for future contestants, anyone who wants to reproduce this car, and readers interested in camera-based line-following algorithms.

---

## 简体中文

### 1. 项目简介

本仓库是 2024 年全国大学生智能汽车竞赛双车组 **B 车**的完整工程与资料。B 车通过 CH9143 无线串口与 A 车/上位机通信，完成发车、出库、摄像头循迹、S 弯、斑马线停车、单边桥、直角转弯、障碍物避让等任务。

车辆采用 **三轮结构**：后轴左右两个直流电机独立驱动，前部为从动支撑轮。转向完全依靠左右后轮的 **差速** 实现：左轮比右轮快则右转，右轮比左轮快则左转。没有舵机，也没有转向机构。

本车最终获得 **安徽省三等奖**。比赛中的主要失误发生在强光环境下：摄像头图像二值化出现全白/全黑，导致循迹失效。这个问题在下面的“已知问题与改进建议”中有详细分析，后续同学可以优先修正 `Threshold_filter()`、`getThreshold()` 和 `ImageBinarization()` 相关逻辑。

### 2. 仓库结构

```text
.
├─ firmware/
│  ├─ Temp_Modified/                 # 实际参赛工程（MounRiver Studio 工程）
│  │  ├─ USER/
│  │  │  ├─ src/main.c               # 图像处理、赛道元素识别、运动决策主逻辑
│  │  │  ├─ src/isr.c                # 中断服务函数（DVP、UART、定时器等）
│  │  │  └─ inc/main.h、isr.h        # 宏定义、全局变量、函数声明
│  │  ├─ CODE/                       # 板级/硬件封装：LED、ADC、按键、电机、UART、RGB 等
│  │  └─ MounRiver/                  # MounRiver Studio 工程文件、链接脚本、下载配置
│  └─ Libraries/
│     ├─ Device/                     # CH32V30x 官方 SDK、启动文件、RISC-V Core
│     ├─ doc/                         # 空目录占位，保持 .project 的链接有效
│     ├─ hefei_libraries/            # HFUT 底层驱动：GPIO、PWM、UART、TIM、DVP、I2C 等
│     └─ hefei_peripheral/           # 外设驱动：MT9V03X 摄像头、IPS114 屏幕、HC-SR04、电机、无线串口等
├─ docs/
│  ├─ reference/
│  │  └─ image_processing_reference.txt   # 额外的图像处理参考代码（Otsu、Sobel、八邻域边界等）
│  └─ vendor/
│     ├─ learning/                   # CH32V307 开发板、转接板、电机驱动板用户手册
│     ├─ mcu/                        # CH32V30x 数据手册、参考手册、RISC-V 手册
│     ├─ peripherals/                # 摄像头、蓝牙、TOF、电机驱动、WCH-Link 等器件资料
│     ├─ mounriver/                  # MounRiver Studio 下载链接说明
│     ├─ assembly/                   # 摄像头智能车组装流程 PPT
│     └─ 说明.txt                    # 原厂资料包说明
├─ tools/
│  ├─ 串口调试助手.exe
│  └─ 玲珑眼摄像头上位机V1.0.exe
├─ README.md
├─ .gitignore
└─ .gitattributes
```

几个容易混淆的目录：

| 路径 | 作用 |
|---|---|
| `firmware/Temp_Modified/MounRiver` | **实际需要打开的 MounRiver Studio 工程目录**（包含 `.cproject`、`.project`、`Link.ld`）。 |
| `firmware/Temp_Modified/USER/src/main.c` | 本车最核心的代码：阈值、二值化、边线提取、中线误差、斑马线/直角/单边桥/障碍物识别、电机输出。 |
| `firmware/Temp_Modified/CODE` | 外设初始化和硬件动作封装，不含赛道识别算法。注意 `car_ligth.c` 是原作者文件名拼写，不是笔误修复后的 `car_light.c`。 |
| `firmware/Libraries/Device` | 芯片厂商 SDK，一般不需要改。 |
| `firmware/Libraries/hefei_libraries` | 合肥工业大学（HFUT）二次封装的底层库。 |
| `firmware/Libraries/hefei_peripheral` | 智能车常用外设驱动，摄像头、屏幕、超声波、电机、无线串口都在这里。 |
| `docs/vendor` | 硬件手册、芯片手册、器件资料、组装说明，不是编译必需文件。 |
| `docs/reference/image_processing_reference.txt` | 原 `图像处理(一)代码.txt`，保留作为图像算法参考，不被工程编译。 |
| `tools` | 上位机/串口工具，按需使用。 |

> 重要：`firmware/Temp_Modified` 和 `firmware/Libraries` 必须保持当前的相对位置。MounRiver 工程通过 `PARENT-2-PROJECT_LOC/Libraries` 引用库文件，单独移动 `Libraries` 会导致工程找不到头文件和源文件。

### 3. 硬件与机械方案

- **车体：** 三轮车。后轴左右两轮分别由一路直流电机驱动，前轮为从动支撑轮。没有舵机，靠左右轮 PWM 占空比差实现转向。
- **主控：** CH32V307VCT6，RISC-V 内核，256 KB Flash / 64 KB SRAM。
- **摄像头：** MT9V034，DVP 并口，配置为 188 × 120 灰度图。代码中 `MT9V03X_DVP_W = 188`、`MT9V03X_DVP_H = 120`。
- **显示：** IPS114 彩色屏，用于显示原始图像、二值化图像、中线、误差、特征状态和调试页面。
- **测距：** HC-SR04 超声波，用于障碍物预判。
- **无线：** CH9143 无线串口，UART6，115200 bps，用于双车通信/发车。
- **灯光：** WS2812B RGB 尾灯，代码在 `car_ligth.c`。
- **按键：** 6 个按键 K1–K6，用于启停、翻页、切换图像显示、显示/隐藏巡线等。
- **编码器：** 库中存在编码器驱动，但本版本主循环没有使用编码器闭环，速度环是开环 PWM。

主要外设引脚（便于接线核对）：

| 外设 | 引脚/资源 |
|---|---|
| 摄像头 DVP | D0–D7：A9、A10、C8、C9、C11、B6、B8、B9；PCLK A6；VSYNC A5；HREF A4 |
| 摄像头配置串口 | UART5，9600 bps，C12/D2 |
| 左电机 | TIM3_CH4 / TIM3_CH3，B1 / B0 |
| 右电机 | TIM2_CH2 / TIM2_CH1，A1 / A0 |
| 超声波 | TRIG B4；ECHO D9；TIM4 计时 |
| IPS114 | SPI2：SCK B13、MOSI B15；RST B7；DC D7；CS D4；BLK D0 |
| 无线串口 | UART6：TX C1、RX C0；RTS E10；115200 bps |
| 按键 | K1–K6：C0–C5，上拉输入 |

### 4. 软件架构与代码逻辑

程序入口在 `firmware/Temp_Modified/USER/src/main.c`：

1. `setup()`：初始化板级外设、电机、超声波、IPS114、按键、ADC、LED、WS2812B、无线串口、中断，然后初始化 MT9V03X 摄像头。
2. `loop()`：轮询按键、刷新显示、等待摄像头帧完成、调用 `processImage()`、显示图像、根据 `race_flag` 决定是否跑车、读取无线指令、定期更新超声波距离。
3. `main()`：`setup()` 后进入 `while(1) loop();`。

主循环中的关键状态：

| 变量 | 含义 |
|---|---|
| `race_flag` | 是否处于比赛/运行状态。K1 或无线指令可切换。 |
| `crossing_flag` / `crossing_stage` | 十字路口标志与阶段。 |
| `zebra_flag` | 斑马线标志。 |
| `is90dg_flag` | 直角弯标志，1 为左直角，2 为右直角。 |
| `bridge_flag` / `bridge_stage` | 单边桥标志与阶段。 |
| `obstruct_flag` | 障碍物标志，1 为触发避障，2 为避障完成/传感器关闭。 |
| `tz_up` / `tz_down` | 单边桥扫描到的上/下突变点调试标志。 |
| `midLineAxis` / `midLineError` | 中线横坐标与偏差。 |

#### 4.1 图像处理流水线

`processImage()` 的核心流程：

```text
原始灰度图
  -> getThreshold()        全局 Otsu 阈值
  -> Threshold_filter()    三点加权滤波
  -> ImageBinarization()   二值化：>= 阈值置 0xFF，否则 0x00
  -> LinesGenerate()       从底部向上扫描左右边界，合成中线
  -> getRoadError_Default() 取中线参考点附近 5 行平均，计算横向误差
  -> isZebra / is90Dg / isSingleBridge / isObstruct
  -> Repair_and_RefreshData() 必要时补线并重新计算误差
```

- `getThreshold()`：对整幅图隔行隔列采样，统计灰度直方图，用 Otsu 类间方差最大化求阈值。代码还把阈值限制在 `90 < threshold < 130`，超出范围就沿用 `last_threshold`。
- `Threshold_filter()`：三点滑动队列，按 `0.70/0.20/0.10` 加权，只有输入在 50–150 之间才入队。
- `ImageBinarization()`：逐像素全局阈值二值化。
- `LinesGenerate()`：从图像最底行开始，以 `IMAGELEN/2` 为种子，向左找白到黑的下降沿作为左边界，向右找白到黑的下降沿作为右边界，然后取左右平均作为中线；下一行用上一行的中线作为搜索起点，逐行向上传播。
- `getRoadError_Default()`：在 `YPOS_REFERENCE = IMAGEWID/2 + 30 = 90` 附近取 5 行中线坐标求平均，减去图像中心 `IMAGELEN/2 = 94`，得到 `midLineError`。

#### 4.2 运动控制

`Car_run_with_error()` 是一个简单的比例控制：

```c
add_value = error * (basic_speed * speed_param + error_param);
motor_motion(basic_speed + add_value,
             basic_speed - add_value,
             SPEED_PARAM_INIT);
```

- `SPEED_REF = 3500`：基础 PWM。
- `SPEED_PARAM_INIT = 1.06`：右电机补偿系数，用来抵消左右电机/机械差异。
- `run_speed_param_compensate = 0.008`：速度前馈系数，速度越高转向增益越大。
- `run_error_param = 1.20`：误差增益。
- `motor_motion()` 里的 `speed_param` 只乘到右轮，因此 `SPEED_PARAM_INIT` 本质上是右轮比例补偿。
- 本版本没有编码器速度闭环；`motor_forward()` 直接写 PWM 占空比，属于开环控制。

#### 4.3 显示与调试

`displayInfo(page)` 支持 4 个页面：

- Page 0：中线横坐标 `M`、偏差 `Err`。
- Page 1：左/右/双侧丢线计数 `Lm`、`Rm`、`Dm`。
- Page 2：十字阶段 `Cs`、超声波距离 `Dis`、障碍物标志 `Obs`。
- Page 3：单边桥标志 `Bf`、阶段 `Bs`、上/下突变点 `U/D`。

按键：

- K1：启动/停止。
- K2 / K4：上一页 / 下一页。
- K3：显示/隐藏巡线定位线。
- K5：显示原始灰度图。
- K6：显示处理后的二值化图。

### 5. 关键赛道元素算法

#### 5.1 单边桥逻辑

单边桥的本质是：**赛道一侧的边线突然消失，车辆需要贴着桥面一侧行驶，并在桥面上放弃普通巡线**。代码用 `bridge_flag` 和 `bridge_stage` 做状态机。

`isSingleBridge()`：

- **阶段 0（未上桥）**：从图像底部向上扫描中线 `M_ARR`，寻找向左的突变：
  - `M_ARR[i] - M_ARR[i-2] >= 8`
  - 前后几行变化很小（<= 2/3）
  - 然后在第 60 行统计黑到白/白到黑的边沿数量，满足 `falling == 1 && rising == 2` 时，认为发现单边桥入口。
  - 置 `tz_down = 1`、`bridge_flag = 1`、`bridge_stage = 1`。
- **阶段 1（对准）**：`bridgeAdjust()` 先向右打角，再回正，再直行一小段，进入阶段 2。
- **阶段 2（桥上补线）**：`isSingleBridge()` 继续在上半图像寻找向右的突变。同时 `Repair_and_RefreshData()` 执行补线：
  - 复制左边界；
  - 用底部右侧边界的 3 个点通过 `getAverageK()` 拟合斜率；
  - 用 `editLineMap_K()` 把右边界向上延伸；
  - `reGenerate_MidLine()` 重新合成中线；
  - 用 `getRoadError_Default(..., 47, ...)` 以 `x = 47` 为新的中线参考，让车往桥面一侧靠。
  - 如果突变点已经比较高（`i < IMAGEWID-6`），说明车已经上桥，置 `tz_up = 0`、`bridge_stage = 3`，放弃巡线直行。
- **阶段 3（下桥）**：`bridgeAdjust()` 给两段固定 PWM 直行，然后清空 `bridge_stage`、`bridge_flag`、`tz_down`、`tz_up`。

这段单边桥 CV 逻辑的详细图文说明见作者教程：  
**<https://snowmiku-home.top/2024/07/18/post-2024718/>**

> 注意：单边桥的“补线”只补一侧，且参考中线被强制设为 47，这会让车明显偏向桥面一侧。这个偏置是调试出来的经验值，不是通用值，换车/换摄像头安装位置后必须重新标定。

#### 5.2 斑马线逻辑

`isZebra()` 只做一件事：在图像中间行 `OutImage[IMAGEWID/2][i]` 上，从左到右统计 **黑到白** 的跳变次数：

```c
if (OutImage[IMAGEWID/2][i] == 0x00 && OutImage[IMAGEWID/2][i+1] == 0xFF)
    down_side_count++;
```

当 `down_side_count >= 7` 时：

```c
zebra_flag = 1;
if (race_flag) key_value_to_process = 0x01;
```

`key_value_to_process = 0x01` 会复用 K1 的处理逻辑：如果正在跑车，就切换 `race_flag = 0` 并让电机停转。因此斑马线识别的最终效果是 **立即停车**。

特点与风险：

- 只扫一行，速度快，但抗干扰弱。
- 没有做多帧确认，单帧满足 7 个跳变就会停车。
- 虚线、赛道边缘、强光造成的黑白噪声都有可能误触发。
- 图像层直接写 `key_value_to_process`，把“识别”和“按键控制”耦合在一起，后续建议改成独立的 `stop_request` 状态。

#### 5.3 直角转弯逻辑

`is90Dg()` 用“丢线行数 + 顶部/底部黑白比例”判断直角：

1. 用 `calculateMissLine()` 统计整幅图左/右边界丢失的行数。
2. 如果右侧丢线 `rm >= 93` 且 `rm - lm >= 20`，预判为右直角；左侧满足 `lm >= 93` 且 `lm - rm >= 20`，预判为左直角。
3. 扫描图像第 5 行黑点数量 `b_count`，第 `IMAGEWID-5` 行白点数量 `w_count`。
4. 如果 `b_count >= 140 && w_count >= 130`，确认是直角弯。
5. `quarterTurn()` 执行固定 320 ms 的差速转向：
   - 左直角：`motor_motion(1800, 3800, ...)`，左轮慢、右轮快。
   - 右直角：`motor_motion(3800, 1800, ...)`，左轮快、右轮慢。
6. 转向后清 `is90dg_flag`，回到正常巡线。

这个逻辑的问题是：**转向角度完全由 320 ms 的时间决定**。电池电压、地面摩擦、轮胎磨损、起始姿态都会影响实际转过的角度，容易出现转多或转少。后续建议用陀螺仪/编码器做角度闭环，或者至少做多段速度补偿。

#### 5.4 十字路口与补线逻辑（当前版本未启用）

`isCrossing()`、`crossing_judge_up_point()`、`crossing_judge_down_point()` 和 `Repair_and_RefreshData()` 的 `crossing_stage` 1–4 分支实现了完整的十字路口四点定位和补线：

- 上拐点/下拐点分别用“连续几行变化很小、再连续几行变化很大”的模板检测。
- 根据左右下角点是否存在，分为 4 种补线情况：
  1. 两侧都正常：直接连接上下拐点。
  2. 右下角缺失：左侧正常连接，右侧用斜率 `k2` 外推。
  3. 左下角缺失：右侧正常连接，左侧用斜率 `k1` 外推。
  4. 两侧下角都缺失：两侧都用斜率外推。
- `getAverageK()` 用 3 个点做最小二乘拟合斜率，`editLineMap_K()` 按斜率补线，`reGenerate_MidLine()` 重新合成中线。

**重要：当前 `processImage()` 并没有调用 `isCrossing()`。**  
所以 `crossing_flag` 在当前版本里基本不会由图像处理置位，十字路口补线逻辑处于“保留但未接线”的状态。`Repair_and_RefreshData()` 目前实际主要服务于单边桥阶段 2。后续同学如果要用十字补线，需要在 `processImage()` 里先调用 `isCrossing()`，再保证满足条件时调用 `Repair_and_RefreshData()`。

#### 5.5 障碍物避让逻辑

`isObstruct()` 先用图像做预判：

- 沿中线从下往上扫描，找白到黑的突变点；
- 统计中线列上的白点数 `white_count` 和黑点数 `black_count`；
- 如果突变点高度在 15–50 行、白点 >= 70、黑点 >= 25，则 `prejudge_flag = 1`。

然后结合超声波 `Wave_distance`：

- 如果 `Wave_distance` 在 3900–5500 之间，且图像预判成立，置 `obstruct_flag = 1`；
- 如果超声波返回 0（错误），则只依赖图像预判触发。

`AvoidObs()` 是一段 **硬编码的固定动作序列**：右转、直行、左转、直行、再右转等，最后置 `obstruct_flag = 2`。主循环在 `obstruct_flag == 2` 时会关闭超声波刷新，把 `Wave_distance` 固定为 1。也就是说，一辆车一次比赛只避障一次，之后不再检测障碍物。

### 6. 编译、下载与运行

1. 安装 **MounRiver Studio (MRS)**，版本建议 1.9.x 或兼容版本；安装 WCH-Link 驱动。
2. 用 MRS 打开 `firmware/Temp_Modified/MounRiver`（这个目录里有 `.project`）。
3. 保持以下相对结构不变：

```text
firmware/
├─ Temp_Modified/
│  ├─ CODE/
│  ├─ USER/
│  └─ MounRiver/
└─ Libraries/
   ├─ Device/
   ├─ hefei_libraries/
   └─ hefei_peripheral/
```

4. 编译工程。生成文件在 `MounRiver/obj`，该目录已被 `.gitignore` 排除，不属于源码。
5. 连接 WCH-Link，下载程序。
6. 上电后：
   - K1 启动/停止；
   - K2/K4 翻页；
   - K3 显示/隐藏巡线；
   - K5 原始图；
   - K6 二值化图。
7. 无线发车：A 车/上位机通过 CH9143 发送字符 `'r'`，B 车收到后回 `'r'`，执行 `Car_left_out()` 出库，再回 `'s'` 表示成功，并置 `race_flag = 1`。

> 源码注释使用 GBK/GB2312 编码。仓库通过 `.gitattributes` 的 `working-tree-encoding=GB18030` 让 GitHub 以 UTF-8 显示，同时本地检出仍保持 GBK，方便 MounRiver 打开。若本地编辑器显示乱码，请把文件编码切换为 GBK/GB18030。

### 7. 已知问题与改进建议

这一节是给后续同学最重要的部分。很多问题不是“代码能不能跑”，而是“换场地、换电池、换光照后还能不能稳定跑”。

#### 7.1 强光下二值化失效（比赛失误的主要原因）

现象：强光照射赛道时，摄像头画面过曝，二值化结果全白或全黑，`LinesGenerate()` 找不到边线，中线误差乱跳，车直接冲出赛道。

原因分析：

- `getThreshold()` 是 **全局 Otsu**，默认整幅图只有一个阈值。强光会让图像直方图严重偏移，Otsu 可能选到一个不合适的阈值。
- 代码把阈值硬限制在 `90 < threshold < 130`，超出范围就沿用 `last_threshold`。强光下如果真实阈值已经不在这个区间，代码仍然使用旧阈值，导致全白/全黑。
- `Threshold_filter()` 只接受 50–150 的输入，且没有“无效帧”概念；连续几帧异常时，它会继续使用队列里的旧值，响应滞后。
- `getThreshold()` 里的 `if (deltaTmp < deltaMax) break;` 可能在类间方差曲线的局部下降处提前退出，进一步降低阈值稳定性。
- 没有曝光控制/增益控制，也没有对“白点比例/黑点比例”做饱和检测。

建议修正方向：

1. **先控制摄像头曝光**：用 `mt9v03x_set_exposure_time_dvp()` 或自动曝光，让画面不要过曝；必要时加偏振片/遮光罩。
2. **改自适应/局部阈值**：分块 Otsu、Sauvola、局部均值阈值，或者直接用 Sobel/边缘跟踪代替全局二值化。
3. **增加无效帧检测**：统计二值化后的白点比例，如果 > 95% 或 < 5%，判定该帧无效，复用上一帧的边线/中线，而不是直接用错误阈值。
4. **重写 `Threshold_filter()`**：不要只做三点加权；可以加入中值滤波、异常值剔除、基于图像均值/方差的动态门限、多帧投票。
5. **去掉硬编码 90–130 的限制**，或者改成“软约束 + 置信度”，并记录每帧阈值用于调试。
6. **保留原始灰度图**，在 IPS 上显示阈值、白点比例、有效帧标志，方便现场排查。

#### 7.2 特征识别顺序与单帧决策

`processImage()` 里 `while (i < 1)` 的结构决定了每帧只优先检查一个特征：斑马线 -> 直角 -> 单边桥 -> 障碍物。高速时可能错过特征，或者斑马线误判后直接停车，后面的直角/单边桥根本来不及判断。

建议：

- 多帧投票/去抖；
- 把特征检测改成并行返回候选，再由状态机仲裁；
- 给每个特征加“距离/时间窗口”，避免同一元素重复触发。

#### 7.3 固定时间动作依赖电池和地面

`Car_left_out()`、`quarterTurn()`、`bridgeAdjust()`、`AvoidObs()` 都是 `Delay_Ms()` + 固定 PWM 的开环动作。电池电压下降、轮胎打滑、地面摩擦变化都会让实际轨迹变化。

建议：

- 加编码器速度闭环；
- 加陀螺仪角度闭环（直角转弯、出库、单边桥对准）；
- 至少做电压补偿和分段减速。

#### 7.4 十字路口逻辑没有接入

`isCrossing()` 没有被 `processImage()` 调用，`crossing_flag` 不会由图像更新。若后续赛道有十字路口，需要：

1. 在 `processImage()` 中调用 `isCrossing(OutImage, &linedata, &crossing_pos);`
2. 确保 `Repair_and_RefreshData()` 在需要的时候被调用；
3. 检查 `crossing_stage` 的 1–4 分支与当前 `LinesGenerate()` 的输出是否匹配。

#### 7.5 其他代码层面的问题

- `editLineMap_connect()` 中 `k = (x2 - x1) / (y2 - y1);` 的分子分母都是整型，先做整数除法再赋给 `float`，斜率会被截断。应写成 `(float)(x2 - x1) / (float)(y2 - y1)`。
- `getAverageK()` 同样存在整数除法问题，且分母可能为 0。
- `calculateMissLineRatio()` 直接 `return count / (yPos_end - yPos_start);`，整数除法会返回 0 或 1，若使用需要改成浮点。
- `isLineMissed()` 中 `direct_select == LEFT_BOUNDARY | RIGHT_BOUNDARY` 依赖运算符优先级，建议写成 `direct_select == (LEFT_BOUNDARY | RIGHT_BOUNDARY)`。
- `Draw_Locationline()` 的循环从 `i = IMAGEWID` 开始，访问 `Left_line_array[IMAGEWID]` 会越界（合法下标是 0–119），应改为 `i = IMAGEWID - 1`。
- `isZebra()` 在图像处理层直接写 `key_value_to_process`，建议改成独立的停车请求变量。
- `isObstruct()` 的超声波读取 `ultra_get_distance()` 里有阻塞式 `while` 等待 ECHO，传感器异常时可能卡住主循环；`g_error_flag` 置位后也不会自动恢复。
- `pwm_duty()` 内部用 `uint16_t match_temp` 保存比较值，占空比过大时可能溢出；虽然本车使用的 3500–3800 不会触发，但后续提高速度时要注意。
- `main.c` 很大，建议按“图像、特征、控制、显示”拆分文件，便于维护。
- 当前没有单元测试/仿真，所有验证都在实车赛道上完成；修改阈值和特征参数后必须实车复测。

### 8. 仓库整理说明

- 原目录约 **96.9 MB / 3159 个文件**；整理后仓库约 **23.6 MB / 167 个文件**。
- 已排除（移动到仓库外的 `D:\Dev\智能车竞赛_excluded`，可确认无误后删除）：
  - MounRiver 构建产物：`obj/`、`.o`、`.d`、`.elf`、`.hex`、`.lst`、`.map`、`.prefs`、`.settings` 等；
  - 重复的库/模板/示例：`File_HFUT/3.Libraries`、`File_HFUT/4.Demo`、`File_HFUT/00-Templete`、`项目文件夹/Seekfree_CH32V307VCT6_Opensource_Library`；
  - 旧版本的 `Hardware` 代码副本（实际使用的是 `Temp_Modified/CODE`）；
  - 空的 `一些代码的备份.txt`；
  - 冗余压缩包 `玲珑眼摄像头上位机V1.0.rar`（已保留解压后的 exe）；
  - 原 `File_HFUT/7.工具软件` 中的工具已移动到 `tools/`；
  - 原 `File_HFUT` 的文档已移动到 `docs/vendor/`。
- 当前仓库没有超过 200 MB 的文件；最大文件约 **6.2 MB**（`docs/vendor/mcu/CH32FV2x_V3xRM.PDF`）。因此不需要 Git LFS。
- 未发现 `.lnk`/`.url` 快捷方式；`.gitignore` 已加入 `*.lnk`、`*.url` 规则，防止以后误提交。
- `.gitignore` 同时忽略常见大文件/压缩包（`*.rar`、`*.zip`、`*.7z`、`*.iso`、视频等），但 GitHub 不能按“文件大小”自动忽略；如果以后要加入超过 100 MB 的单个文件，请使用 Git LFS 或外链。

### 9. 许可与致谢

- `firmware/Libraries/Device` 来自 WCH CH32V30x 官方 SDK。
- `firmware/Libraries/hefei_libraries`、`firmware/Libraries/hefei_peripheral`、`docs/vendor` 来自 HFUT/合肥工业大学智能车资料包及对应器件厂商。
- 第三方库请遵循其原始许可证（例如 GPLv3，具体见源码头注释和 `docs` 中的说明）。
- 本仓库的应用层代码用于学习、交流和后续参赛参考，请保留原作者与资料出处。

---

## English

### 1. Overview

This repository contains the complete project and documentation for **Car B** of the dual-car group in the **2024 National University Student Intelligent Car Competition**. Car B communicates with Car A / a host PC over a CH9143 wireless UART, and performs: start, leaving the parking area, camera-based line following, S-curves, stopping at a zebra crossing, single-side bridge crossing, 90-degree turns, and obstacle avoidance.

The car is a **three-wheel platform**: the two rear wheels are driven independently by two DC motors, and the front wheel is a passive support/caster wheel. Steering is achieved purely by **differential speed** between the rear wheels. There is no steering servo.

The team won the **Third Prize of Anhui Province**. The main failure during the competition happened under strong sunlight: the camera image became overexposed, the binarization output became all white or all black, and line following failed. The “Known Issues” section below analyzes this in detail. Future students should prioritize fixing `getThreshold()`, `Threshold_filter()`, and `ImageBinarization()`.

### 2. Repository Layout

```text
.
├─ firmware/
│  ├─ Temp_Modified/                 # Actual competition project (MounRiver Studio)
│  │  ├─ USER/
│  │  │  ├─ src/main.c               # Image processing, track features, motion decisions
│  │  │  ├─ src/isr.c                # Interrupt handlers (DVP, UART, timers)
│  │  │  └─ inc/main.h, isr.h        # Macros, globals, prototypes
│  │  ├─ CODE/                       # Board/hardware wrappers: LED, ADC, keys, motors, UART, RGB
│  │  └─ MounRiver/                  # MRS project, linker script, download config
│  └─ Libraries/
│     ├─ Device/                     # CH32V30x official SDK, startup, RISC-V core
│     ├─ doc/                         # Empty placeholder to keep the .project link valid
│     ├─ hefei_libraries/            # HFUT low-level drivers: GPIO, PWM, UART, TIM, DVP, I2C
│     └─ hefei_peripheral/           # MT9V03X camera, IPS114, HC-SR04, motor, wireless UART
├─ docs/
│  ├─ reference/
│  │  └─ image_processing_reference.txt   # Extra image-processing reference code
│  └─ vendor/
│     ├─ learning/                   # Board / adapter / motor-driver manuals
│     ├─ mcu/                        # CH32V30x datasheets, reference manual, RISC-V manual
│     ├─ peripherals/                # Camera, Bluetooth, TOF, motor driver, WCH-Link docs
│     ├─ mounriver/                  # MounRiver Studio download note
│     ├─ assembly/                   # Camera-car assembly PPT
│     └─ 说明.txt                    # Original vendor package readme
├─ tools/
│  ├─ 串口调试助手.exe               # Serial assistant
│  └─ 玲珑眼摄像头上位机V1.0.exe     # Camera PC GUI
├─ README.md
├─ .gitignore
└─ .gitattributes
```

Key points:

| Path | Responsibility |
|---|---|
| `firmware/Temp_Modified/MounRiver` | **The MRS project directory to open** (contains `.cproject`, `.project`, `Link.ld`). |
| `firmware/Temp_Modified/USER/src/main.c` | Core algorithm: threshold, binarization, boundary extraction, midline error, zebra/90-degree/single-side-bridge/obstacle detection, motor output. |
| `firmware/Temp_Modified/CODE` | Board-level init and hardware action wrappers; no track-recognition algorithm. `car_ligth.c` is the original author's filename typo. |
| `firmware/Libraries/Device` | Vendor SDK; normally no need to modify. |
| `firmware/Libraries/hefei_libraries` | HFUT second-level low-level library. |
| `firmware/Libraries/hefei_peripheral` | Camera, screen, ultrasonic, motor, wireless UART drivers. |
| `docs/vendor` | Hardware/device manuals and assembly guide; not required for compilation. |
| `docs/reference/image_processing_reference.txt` | Original `图像处理(一)代码.txt`, kept as an image-algorithm reference. |
| `tools` | PC-side tools, use as needed. |

> Important: keep `firmware/Temp_Modified` and `firmware/Libraries` in their current relative positions. The MRS project references libraries through `PARENT-2-PROJECT_LOC/Libraries`; moving `Libraries` alone will break the include paths.

### 3. Hardware and Mechanical Design

- **Chassis:** three wheels; rear left/right wheels are independently driven; front wheel is passive. No steering servo; heading is controlled by rear-wheel PWM difference.
- **MCU:** CH32V307VCT6, RISC-V, 256 KB Flash / 64 KB SRAM.
- **Camera:** MT9V034, DVP parallel interface, 188 × 120 grayscale.
- **Display:** IPS114 color LCD for raw image, binary image, midline, error, feature state, and debug pages.
- **Ranging:** HC-SR04 ultrasonic sensor for obstacle pre-judgement.
- **Wireless:** CH9143 wireless UART, UART6, 115200 bps, for dual-car communication/start.
- **Lighting:** WS2812B RGB tail lights (`car_ligth.c`).
- **Keys:** six keys K1–K6 for start/stop, page switching, image display, line overlay.
- **Encoder:** the library contains encoder drivers, but this version does not use encoder feedback in the main loop; motor speed is open-loop PWM.

Main pin/resource mapping:

| Peripheral | Pin/Resource |
|---|---|
| Camera DVP | D0–D7: A9, A10, C8, C9, C11, B6, B8, B9; PCLK A6; VSYNC A5; HREF A4 |
| Camera config UART | UART5, 9600 bps, C12/D2 |
| Left motor | TIM3_CH4 / TIM3_CH3, B1 / B0 |
| Right motor | TIM2_CH2 / TIM2_CH1, A1 / A0 |
| Ultrasonic | TRIG B4; ECHO D9; TIM4 timing |
| IPS114 | SPI2: SCK B13, MOSI B15; RST B7; DC D7; CS D4; BLK D0 |
| Wireless UART | UART6: TX C1, RX C0; RTS E10; 115200 bps |
| Keys | K1–K6: C0–C5, pull-up input |

### 4. Software Architecture and Code Flow

Entry point: `firmware/Temp_Modified/USER/src/main.c`.

1. `setup()`: initializes board, motors, ultrasonic, IPS114, keys, ADC, LEDs, WS2812B, wireless UART, interrupts, then MT9V03X camera.
2. `loop()`: polls keys, refreshes display, waits for a camera frame, calls `processImage()`, displays image, runs the car if `race_flag` is set, reads wireless commands, and periodically updates ultrasonic distance.
3. `main()`: calls `setup()` and then `while (1) loop();`.

Important state variables:

| Variable | Meaning |
|---|---|
| `race_flag` | Running/competition state; toggled by K1 or wireless command. |
| `crossing_flag` / `crossing_stage` | Crossing flag and stage. |
| `zebra_flag` | Zebra-crossing flag. |
| `is90dg_flag` | 90-degree turn flag: 1 = left, 2 = right. |
| `bridge_flag` / `bridge_stage` | Single-side bridge flag and stage. |
| `obstruct_flag` | 1 = trigger avoidance, 2 = avoidance done / sensor disabled. |
| `tz_up` / `tz_down` | Debug flags for bridge jump points. |
| `midLineAxis` / `midLineError` | Midline x-coordinate and error. |

#### 4.1 Image Pipeline

```text
raw grayscale
  -> getThreshold()        global Otsu threshold
  -> Threshold_filter()    3-tap weighted filter
  -> ImageBinarization()   >= threshold -> 0xFF, otherwise 0x00
  -> LinesGenerate()       bottom-up left/right boundary scan, midline
  -> getRoadError_Default() average of 5 rows near the reference row
  -> isZebra / is90Dg / isSingleBridge / isObstruct
  -> Repair_and_RefreshData() line completion when needed
```

- `getThreshold()`: samples every other row/column, builds a grayscale histogram, and maximizes the Otsu between-class variance. The result is constrained to `90 < threshold < 130`; otherwise `last_threshold` is reused.
- `Threshold_filter()`: three-sample queue with `0.70/0.20/0.10` weights; only inputs in 50–150 are accepted.
- `ImageBinarization()`: global per-pixel thresholding.
- `LinesGenerate()`: starts at the bottom row with `IMAGELEN/2` as the seed, scans left for a white-to-black falling edge and right for a white-to-black falling edge, then uses the average as the midline. The next row uses the previous midline as its search origin, propagating upward.
- `getRoadError_Default()`: averages the midline x-coordinate over 5 rows near `YPOS_REFERENCE = IMAGEWID/2 + 30 = 90`, then subtracts the image center `IMAGELEN/2 = 94` to produce `midLineError`.

#### 4.2 Motion Control

`Car_run_with_error()` is a simple proportional controller:

```c
add_value = error * (basic_speed * speed_param + error_param);
motor_motion(basic_speed + add_value,
             basic_speed - add_value,
             SPEED_PARAM_INIT);
```

- `SPEED_REF = 3500`: base PWM.
- `SPEED_PARAM_INIT = 1.06`: right-motor compensation factor.
- `run_speed_param_compensate = 0.008`: speed feed-forward gain.
- `run_error_param = 1.20`: error gain.
- `motor_motion()` applies `speed_param` only to the right wheel, so `SPEED_PARAM_INIT` is effectively a right-wheel scale factor.
- There is no encoder speed loop in this version; `motor_forward()` writes PWM directly, so the control is open-loop.

#### 4.3 Display and Debugging

`displayInfo(page)` supports four pages:

- Page 0: midline `M`, error `Err`.
- Page 1: left/right/both missing-line counts `Lm`, `Rm`, `Dm`.
- Page 2: crossing stage `Cs`, ultrasonic distance `Dis`, obstacle flag `Obs`.
- Page 3: bridge flag `Bf`, stage `Bs`, up/down jump flags `U/D`.

Keys:

- K1: start/stop.
- K2 / K4: previous/next page.
- K3: show/hide line overlay.
- K5: raw grayscale image.
- K6: processed binary image.

### 5. Key Track-Feature Algorithms

#### 5.1 Single-Side Bridge

The single-side bridge is detected when one side of the track boundary suddenly disappears. The car must approach the bridge side and temporarily abandon normal line following on the bridge. The code uses `bridge_flag` and `bridge_stage` as a state machine.

`isSingleBridge()`:

- **Stage 0 (before bridge)**: scan the midline `M_ARR` from the bottom upward and look for a leftward jump:
  - `M_ARR[i] - M_ARR[i-2] >= 8`
  - small changes in the previous/following rows (<= 2/3)
  - then count black-to-white/white-to-black edges on row 60; if `falling == 1 && rising == 2`, the bridge entrance is confirmed.
  - set `tz_down = 1`, `bridge_flag = 1`, `bridge_stage = 1`.
- **Stage 1 (align)**: `bridgeAdjust()` turns right, straightens, and moves forward briefly, then enters stage 2.
- **Stage 2 (line repair on bridge)**: `isSingleBridge()` continues to search for a rightward jump in the upper image. At the same time, `Repair_and_RefreshData()` repairs the line:
  - copy the left boundary;
  - fit the right boundary slope from the bottom three points using `getAverageK()`;
  - extend the right boundary upward with `editLineMap_K()`;
  - regenerate the midline with `reGenerate_MidLine()`;
  - call `getRoadError_Default(..., 47, ...)` with `x = 47` as the new midline reference, biasing the car toward the bridge side.
  - if the jump point is already high (`i < IMAGEWID-6`), the car is on the bridge: set `tz_up = 0`, `bridge_stage = 3`, and drive straight without line following.
- **Stage 3 (leave bridge)**: `bridgeAdjust()` applies two fixed PWM segments, then clears `bridge_stage`, `bridge_flag`, `tz_down`, and `tz_up`.

The detailed illustrated explanation of this single-side bridge CV logic is in the author's tutorial:  
**<https://snowmiku-home.top/2024/07/18/post-2024718/>**

> Note: the repair only fills one side, and the midline reference is forced to 47, which deliberately biases the car toward the bridge. This is a tuned empirical value; it must be recalibrated when the car, camera position, or track changes.

#### 5.2 Zebra Crossing

`isZebra()` scans the middle image row `OutImage[IMAGEWID/2][i]` from left to right and counts **black-to-white** transitions:

```c
if (OutImage[IMAGEWID/2][i] == 0x00 && OutImage[IMAGEWID/2][i+1] == 0xFF)
    down_side_count++;
```

When `down_side_count >= 7`:

```c
zebra_flag = 1;
if (race_flag) key_value_to_process = 0x01;
```

`key_value_to_process = 0x01` reuses the K1 handler: if the car is running, it sets `race_flag = 0` and stops the motors. So the final effect of zebra detection is **immediate stop**.

Risks:

- Only one row is scanned; it is fast but noise-sensitive.
- No multi-frame confirmation; a single frame with 7 transitions stops the car.
- Dashed lines, track edges, and strong-light noise can cause false triggers.
- The image layer writes `key_value_to_process` directly, coupling recognition with key handling. A separate `stop_request` state is recommended.

#### 5.3 90-Degree Turn

`is90Dg()` uses “missing-line rows + top/bottom black/white ratio”:

1. `calculateMissLine()` counts missing left/right boundary rows over the whole image.
2. If right missing `rm >= 93` and `rm - lm >= 20`, predict a right 90-degree turn; if left missing `lm >= 93` and `lm - rm >= 20`, predict a left turn.
3. Count black pixels on row 5 (`b_count`) and white pixels on row `IMAGEWID-5` (`w_count`).
4. If `b_count >= 140 && w_count >= 130`, confirm the 90-degree turn.
5. `quarterTurn()` applies a fixed 320 ms differential pulse:
   - left turn: `motor_motion(1800, 3800, ...)`;
   - right turn: `motor_motion(3800, 1800, ...)`.
6. Clear `is90dg_flag` and return to normal line following.

The main weakness is that **the turn angle is determined entirely by 320 ms of open-loop timing**. Battery voltage, floor friction, tire wear, and initial pose all affect the actual angle. Use a gyro/encoder angle loop, or at least voltage compensation and multi-stage speed scheduling.

#### 5.4 Crossing and Line Repair (Not Wired in This Version)

`isCrossing()`, `crossing_judge_up_point()`, `crossing_judge_down_point()`, and the `crossing_stage` 1–4 branches in `Repair_and_RefreshData()` implement full four-point crossing localization and line repair:

- Up/down corner points are detected with templates: several rows with small changes followed by several rows with large changes.
- Depending on which lower corners exist, there are four repair cases:
  1. both sides normal: connect upper and lower points directly;
  2. right lower corner missing: connect left side, extrapolate right side with slope `k2`;
  3. left lower corner missing: connect right side, extrapolate left side with slope `k1`;
  4. both lower corners missing: extrapolate both sides.
- `getAverageK()` fits a slope from three points; `editLineMap_K()` fills the line; `reGenerate_MidLine()` recomputes the midline.

**Important: `processImage()` does not call `isCrossing()` in the current version.**  
Therefore `crossing_flag` is not updated by the image pipeline, and the crossing repair logic is essentially dormant. `Repair_and_RefreshData()` currently mainly serves single-side bridge stage 2. If you want crossing repair, call `isCrossing()` in `processImage()` first, and make sure `Repair_and_RefreshData()` is called when needed.

#### 5.5 Obstacle Avoidance

`isObstruct()` first uses the image:

- scan upward along the midline for a white-to-black transition;
- count white and black pixels along the midline column;
- if the transition row is 15–50, `white_count >= 70`, and `black_count >= 25`, set `prejudge_flag = 1`.

Then it combines the ultrasonic distance:

- if `Wave_distance` is 3900–5500 and the image pre-judgement holds, set `obstruct_flag = 1`;
- if the ultrasonic returns 0 (error), rely on the image pre-judgement alone.

`AvoidObs()` is a **hard-coded fixed action sequence** (right, straight, left, straight, right, etc.), ending with `obstruct_flag = 2`. The main loop then disables ultrasonic refresh and forces `Wave_distance = 1`. In other words, the car avoids only one obstacle per run and never re-enables the sensor.

### 6. Build, Flash, and Run

1. Install **MounRiver Studio (MRS)**, preferably 1.9.x or a compatible version, and the WCH-Link driver.
2. Open `firmware/Temp_Modified/MounRiver` in MRS (this directory contains `.project`).
3. Keep this relative layout unchanged:

```text
firmware/
├─ Temp_Modified/
│  ├─ CODE/
│  ├─ USER/
│  └─ MounRiver/
└─ Libraries/
   ├─ Device/
   ├─ hefei_libraries/
   └─ hefei_peripheral/
```

4. Build the project. Output goes to `MounRiver/obj`, which is ignored by `.gitignore` and is not source.
5. Connect WCH-Link and download.
6. After power-on:
   - K1: start/stop;
   - K2/K4: page switching;
   - K3: toggle line overlay;
   - K5: raw image;
   - K6: binary image.
7. Wireless start: Car A / host PC sends `'r'` over CH9143; Car B replies `'r'`, runs `Car_left_out()`, then replies `'s'` and sets `race_flag = 1`.

> Source comments are GBK/GB2312. The repository uses `.gitattributes` with `working-tree-encoding=GB18030` so GitHub displays UTF-8 while the local checkout remains GBK for MounRiver. If your editor shows garbled Chinese, switch the file encoding to GBK/GB18030.

### 7. Known Issues and Suggested Improvements

This is the most important section for future students. Many issues are not “does it compile”, but “will it still run after changing the venue, battery, or lighting?”

#### 7.1 Binarization Failure Under Strong Light (Main Competition Failure)

Symptom: under strong sunlight, the camera image is overexposed and the binary image becomes all white or all black. `LinesGenerate()` cannot find boundaries, the midline error jumps, and the car leaves the track.

Causes:

- `getThreshold()` uses **global Otsu** with one threshold for the whole image. Strong light shifts the histogram severely, and Otsu may choose an unsuitable threshold.
- The threshold is hard-limited to `90 < threshold < 130`; outside that range, `last_threshold` is reused. Under strong light, the real threshold may be outside this range, so the old threshold is still used and the image becomes all white/black.
- `Threshold_filter()` only accepts 50–150 and has no “invalid frame” concept; it keeps using stale values during consecutive abnormal frames.
- The `if (deltaTmp < deltaMax) break;` in `getThreshold()` may exit early at a local decrease of the between-class variance.
- There is no exposure/gain control and no white/black saturation detection.

Suggested fixes:

1. **Control camera exposure first**: use `mt9v03x_set_exposure_time_dvp()` or auto-exposure, and add a polarizer/hood if necessary.
2. **Use adaptive/local thresholding**: block-wise Otsu, Sauvola, local mean threshold, or edge tracking (Sobel) instead of global binarization.
3. **Add invalid-frame detection**: compute the white-pixel ratio after binarization; if > 95% or < 5%, mark the frame invalid and reuse the previous boundary/midline instead of using a bad threshold.
4. **Rewrite `Threshold_filter()`**: add median filtering, outlier rejection, image mean/variance-based dynamic gating, and multi-frame voting instead of only a 3-tap weighted average.
5. **Remove the hard-coded 90–130 limit**, or turn it into a soft constraint with confidence, and log each frame's threshold for debugging.
6. **Keep the raw grayscale image** and display threshold, white ratio, and valid-frame flag on the IPS for on-site diagnosis.

#### 7.2 Feature Order and Single-Frame Decisions

The `while (i < 1)` structure in `processImage()` means only one feature is checked per frame in priority order: zebra -> 90-degree -> single-side bridge -> obstacle. At high speed, features can be missed, or a false zebra detection can stop the car before later features are evaluated.

Suggestions:

- multi-frame voting/debouncing;
- return feature candidates in parallel and arbitrate in a state machine;
- add distance/time windows so the same element is not triggered repeatedly.

#### 7.3 Fixed-Time Actions Depend on Battery and Floor

`Car_left_out()`, `quarterTurn()`, `bridgeAdjust()`, and `AvoidObs()` are all open-loop `Delay_Ms()` + fixed PWM sequences. Battery voltage drop, wheel slip, and floor friction change the actual trajectory.

Suggestions:

- add encoder speed feedback;
- add gyro angle feedback (90-degree turn, parking exit, bridge alignment);
- at least add voltage compensation and staged deceleration.

#### 7.4 Crossing Logic Is Not Connected

`isCrossing()` is not called by `processImage()`, so `crossing_flag` is not updated by the image pipeline. If a future track has a crossing:

1. call `isCrossing(OutImage, &linedata, &crossing_pos);` in `processImage()`;
2. ensure `Repair_and_RefreshData()` is called when required;
3. verify that the `crossing_stage` 1–4 branches match the current `LinesGenerate()` output.

#### 7.5 Other Code-Level Issues

- In `editLineMap_connect()`, `k = (x2 - x1) / (y2 - y1);` performs integer division before assigning to `float`; the slope is truncated. Use `(float)(x2 - x1) / (float)(y2 - y1)`.
- `getAverageK()` has the same integer-division issue, and its denominator may be zero.
- `calculateMissLineRatio()` returns `count / (yPos_end - yPos_start)` with integer division; cast to float if used.
- In `isLineMissed()`, `direct_select == LEFT_BOUNDARY | RIGHT_BOUNDARY` relies on operator precedence; write `direct_select == (LEFT_BOUNDARY | RIGHT_BOUNDARY)`.
- `Draw_Locationline()` starts at `i = IMAGEWID` and accesses `Left_line_array[IMAGEWID]`, which is out of bounds (valid 0–119); start at `IMAGEWID - 1`.
- `isZebra()` writes `key_value_to_process` from the image layer; use a separate stop-request variable.
- `ultra_get_distance()` in `isObstruct()` has blocking `while` loops waiting for ECHO; a faulty sensor can stall the main loop. `g_error_flag` is never automatically cleared.
- `pwm_duty()` stores the compare value in `uint16_t match_temp`; very large duty values may overflow. The current 3500–3800 range is safe, but higher speeds need care.
- `main.c` is very large; split it into image, feature, control, and display modules for maintainability.
- There are no unit tests or simulation; all validation is on the real track. Re-test after changing threshold or feature parameters.

### 8. Repository Cleanup Notes

- Original folder: about **96.9 MB / 3159 files**; cleaned repository: about **23.6 MB / 167 files**.
- Excluded and moved outside the repository to `D:\Dev\智能车竞赛_excluded` (delete after verification if not needed):
  - MounRiver build outputs: `obj/`, `.o`, `.d`, `.elf`, `.hex`, `.lst`, `.map`, `.prefs`, `.settings`, etc.;
  - duplicate libraries/templates/demos: `File_HFUT/3.Libraries`, `File_HFUT/4.Demo`, `File_HFUT/00-Templete`, `项目文件夹/Seekfree_CH32V307VCT6_Opensource_Library`;
  - legacy `Hardware` code copy (the actual code is `Temp_Modified/CODE`);
  - empty `一些代码的备份.txt`;
  - redundant archive `玲珑眼摄像头上位机V1.0.rar` (the extracted `.exe` is kept);
  - tools from `File_HFUT/7.工具软件` moved to `tools/`;
  - vendor documents from `File_HFUT` moved to `docs/vendor/`.
- No file currently exceeds 200 MB; the largest is about **6.2 MB** (`docs/vendor/mcu/CH32FV2x_V3xRM.PDF`), so Git LFS is not required.
- No `.lnk`/`.url` shortcuts were found; `.gitignore` includes `*.lnk` and `*.url` to prevent accidental commits.
- `.gitignore` also excludes common large files/archives (`*.rar`, `*.zip`, `*.7z`, `*.iso`, videos, etc.). GitHub cannot ignore by file size; for a future single file > 100 MB, use Git LFS or an external link.

### 9. License and Credits

- `firmware/Libraries/Device` comes from the WCH CH32V30x official SDK.
- `firmware/Libraries/hefei_libraries`, `firmware/Libraries/hefei_peripheral`, and `docs/vendor` come from the HFUT (Hefei University of Technology) smart-car package and the corresponding device vendors.
- Third-party libraries retain their original licenses (for example GPLv3; see source headers and `docs`).
- The application-layer code is shared for learning, communication, and future competition reference. Please keep the original author and source credits.
