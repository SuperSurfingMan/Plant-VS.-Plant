# PlantVersus · 植物大战植物

一款基于 **EasyX** 图形库与 **C++** 编写的 2D 双人同屏横版对战小游戏（Windows 平台）。
两名玩家分别操控「婉逗射手」和「龙日葵」，在多层平台间跳跃、射击，先击倒对方的一方获胜。

> 学习与交流项目，详情可见 B 站 up 主 [@Voidmatrix](https://space.bilibili.com/25864506) 的 EasyX 游戏开发系列中的植物明星大乱斗。

---

## 目录

- [游戏玩法](#游戏玩法)
- [项目结构](#项目结构)
- [环境要求](#环境要求)
- [获取并放置美术资源](#获取并放置美术资源)
- [构建与运行](#构建与运行)
- [常见问题](#常见问题)

---

## 游戏玩法

游戏共三个场景：**主菜单 → 选角界面 → 对战界面**。

1. **主菜单**：显示背景与角色跑动动画，按任意键进入选角界面。
2. **选角界面**：
   - 1P 用 <kbd>A</kbd> / <kbd>D</kbd> 切换角色，2P 用 <kbd>←</kbd> / <kbd>→</kbd> 切换角色；
   - 双方选好后按 <kbd>回车</kbd> 开始对战。
3. **对战界面**：双方在同屏的多层平台上移动、跳跃、对战，角色 HP 归零的一方落败，
   随后播放获胜展示动画并自动返回主菜单。

### 键位

| 操作   | 1P（婉逗射手 / 龙日葵） | 2P            |
| ------ | ---------------------- | ------------- |
| 左移   | <kbd>A</kbd>           | <kbd>←</kbd>  |
| 右移   | <kbd>D</kbd>           | <kbd>→</kbd>  |
| 跳跃   | <kbd>W</kbd>           | <kbd>↑</kbd>  |
| 普通攻击 | <kbd>F</kbd>         | <kbd>.</kbd>（句号） |
| 特殊攻击（大招） | <kbd>G</kbd> | <kbd>/</kbd>（斜杠） |

> 每位角色有独立的 HP（生命值）与 MP（能量）条：普通攻击命中造成伤害，MP 积累到 100 后
> 可以释放一次威力更强的特殊攻击（豌逗射手的强化豌豆 / 龙日葵的日光轰炸），释放后 MP 清零。

---

## 项目结构

源码按职能分为四个类别目录：

```text
PlantVersus/                      # 仓库根目录
├── PlantVersus.sln               # Visual Studio 解决方案
├── README.md
└── PlantVersus/                  # VS 工程目录（与 .vcxproj 同级）
    ├── PlantVersus.vcxproj       # 工程文件
    ├── PlantVersus.vcxproj.filters
    ├── game.cpp                  # 程序入口：资源加载、主循环、全局对象
    ├── core/                     # 基础组件
    │   ├── Animation.h           #   帧动画
    │   ├── Atlas.h               #   图集（一组帧图片）
    │   ├── Camera.h              #   镜头（抖动、偏移）
    │   ├── particle.h            #   粒子（跑动/落地特效）
    │   ├── Timer.h               #   计时器（支持等待回调、暂停）
    │   ├── Util.h                #   通用工具（绘制翻转等）
    │   └── Vector2.h             #   二维向量
    ├── scene/                    # 场景与世界
    │   ├── Scene.h               #   场景基类
    │   ├── SceneManager.h        #   场景管理器
    │   ├── MenuScene.h           #   主菜单场景
    │   ├── SelectorScene.h       #   选角场景
    │   ├── GameScene.h           #   对战场景
    │   ├── Platform.h            #   平台（碰撞检测）
    │   └── status_bar.h          #   玩家状态条（HP/MP）
    ├── player/                   # 玩家与角色
    │   ├── Player.h              #   玩家基类（移动/跳跃/攻击/受击）
    │   ├── PlayerID.h            #   玩家编号（1P/2P）
    │   ├── Peashooter_player.h   #   婉逗射手（豌豆射击）
    │   └── Sunflower_player.h    #   龙日葵（日光攻击）
    ├── bullet/                   # 子弹
    │   ├── Bullet.h              #   子弹基类
    │   ├── pea_bullet.h          #   豌豆子弹
    │   ├── sun_bullet.h          #   日光子弹
    │   └── sun_bullet_ex.h       #   强化日光子弹
    └── resources/                # ★ 美术/音频资源（不入 git，需手动下载，见下文）
```

资源文件体积较大，**不在 git 仓库中跟踪**，改用百度网盘分发；
`.gitignore` 已忽略 `PlantVersus/resources/` 目录，克隆或更新仓库不会影响它。

---

## 环境要求

| 项目 | 要求 |
| ---- | ---- |
| 操作系统 | Windows 10 / 11 |
| IDE | Visual Studio 2022（v143 工具集），需勾选 **"使用 C++ 的桌面开发"** 工作负载 |
| 图形库 | [EasyX](https://easyx.cn)（请下载与 VS2022 对应的版本安装） |
| SDK | Windows 10 SDK（10.0，安装 VS 时勾选即可） |
| 平台 | Debug / Release × x64 / Win32 均可，默认推荐 **Debug \| x64** |

> EasyX 安装完成后重启 Visual Studio 即可被识别，无需额外配置工程属性
> （游戏用到的 `graphics.h`、`Winmm.lib` 已在代码中处理）。

---

## 获取并放置美术资源

仓库不含任何图片、音频和字体，运行前必须先下载资源包：

**通过网盘分享的文件：植物大战植物美术资源.zip**
链接：https://pan.baidu.com/s/1fE1uN9Ckr7ZpRUUTMDKRSw?pwd=66xx
提取码：`66xx`

### 放置方式

1. 下载并解压 `植物大战植物美术资源.zip`；
2. 将解压得到的 **`resources` 文件夹**整体放入 **`PlantVersus/PlantVersus/`** 目录
   （即与 `PlantVersus.vcxproj` 同级）；
3. 放好后应满足如下结构（以字体文件和菜单背景为例）：

```text
PlantVersus/PlantVersus/
├── PlantVersus.vcxproj
├── game.cpp
└── resources/
    ├── IPix.ttf                # 游戏像素字体
    ├── menu_background.png
    ├── bgm_menu.mp3
    ├── peashooter_idle_1.png
    └── ……（共 120 余个 png/mp3/wav/ttf 文件）
```

> - 若解压出来直接是散装的图片/音频文件（而不是一个 `resources` 文件夹），请先在
>   `PlantVersus/PlantVersus/` 下手动新建 `resources` 文件夹，再把所有文件放进去。
> - 若仓库克隆时已存在空的 `resources` 目录，直接将解压内容覆盖合并进去即可。
> - 资源文件名与 `game.cpp` 中 `load_game_resources()` 的加载路径一一对应，
>   请**不要重命名**任何文件。

---

## 构建与运行

### 方式一：Visual Studio（推荐）

1. 双击打开根目录的 **`PlantVersus.sln`**；
2. 顶部配置切换为 **Debug \| x64**；
3. 直接按 **F5**（调试运行）或 <kbd>Ctrl</kbd>+<kbd>F5</kbd> 运行。

> 资源通过相对路径 `resources/...` 加载。VS 调试时默认工作目录就是工程目录
> （`PlantVersus/PlantVersus/`），因此按上文放置资源后，F5 即可直接运行，无需其他配置。

### 方式二：命令行构建

已安装 VS2022 的环境下，可在「Developer Command Prompt for VS 2022」中执行：

```bat
msbuild PlantVersus.sln /p:Configuration=Debug /p:Platform=x64 /m
```

构建产物位于 `PlantVersus\x64\Debug\PlantVersus.exe`。

### 直接运行 exe

若不经过 VS、直接双击 `x64\Debug\PlantVersus.exe` 运行，程序的当前目录是 exe 所在目录，
此时需要**把 `resources` 文件夹复制一份到 `PlantVersus\x64\Debug\` 下**（与 exe 同级），
否则资源会加载失败。推荐始终通过 VS 的 F5 方式运行。

---

## 常见问题

**Q：启动后图片/音乐没加载、窗口空白或报错？**
A：绝大多数情况是 `resources` 目录位置不对。请确认 `resources` 与 `PlantVersus.vcxproj`
同级，且目录内文件齐全（如 `IPix.ttf`、`menu_background.png`、`bgm_menu.mp3` 均存在）；
直接运行 exe 的话，`resources` 需与 exe 同级。

**Q：编译报找不到 `graphics.h`？**
A：EasyX 未安装或版本与 VS2022 不匹配，到 [easyx.cn](https://easyx.cn) 下载对应版本安装后重试。

**Q：源码打开后中文注释乱码？**
A：源码使用 GBK（ANSI）编码，请用 Visual Studio 打开；部分跨平台编辑器默认按 UTF-8
解码会显示乱码，但编译不受影响。

**Q：字体显示异常？**
A：游戏通过 `AddFontResourceEx` 私有加载 `resources/IPix.ttf`，请确认该字体文件存在，
且不要在系统未加载字体的情况下于外部工具中预览文本。
