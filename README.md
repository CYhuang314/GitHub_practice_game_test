該 Repository 由 AI 生成，如有任何問題，請直接至 Discussions 討論。

# Git Patch Pusher

推箱子小遊戲

初始版本有 2 個可通關的教學關卡，以及 28 個已完成地圖、但解鎖判定有問題的挑戰關卡。**每修好一個 issue，就多開一關可以玩。** 大多只要改一行，適合第一次 fork、clone、branch、add、commit、push 和 PR。

直接查看 [待解 issues](https://github.com/WgLy/GitHub_practice_game/issues)，認領一題再開始。

這是一款使用 Git 主題命名的推箱子遊戲。真實 Git 操作在 repo 上進行；遊戲不會執行 Git 指令。

## 開始遊玩

需要支援 C11 的 GCC、Clang 或 MSVC；Git 本身不包含 C 編譯器。遊玩不需要 Python、Node、網頁或第三方 C 函式庫。

### Windows

```powershell
cd GitHub_practice_game
.\run.cmd
```

在 CMD 中也可以執行 `run.cmd`。這個入口會啟動 Windows 內建的 PowerShell，以單次執行政策執行 `run.ps1`，自動找到編譯器、重新編譯並啟動遊戲。修改 C 後再執行同一個腳本即可看到成果。

若已能執行 PowerShell 腳本，也可以直接使用：

```powershell
.\run.ps1
```

也可以直接啟動已編譯的版本：

```powershell
.\build\git-game.exe
```

### macOS / Linux

```sh
cd GitHub_practice_game
sh ./run.sh
```

不需要先執行 `chmod`，使用 `sh` 即可。腳本會自動選擇 `cc`、GCC 或 Clang，依目前作業系統編譯。Windows 的 Git Bash 也支援這個入口。

### 初次準備編譯器

| 系統 | 初次準備 |
|---|---|
| Windows | 安裝 [MSYS2](https://www.msys2.org/)，在 UCRT64 終端機執行 `pacman -S --needed mingw-w64-ucrt-x86_64-gcc`；或安裝 Visual Studio Build Tools 的「使用 C++ 的桌面開發」工作負載 |
| macOS | 執行 `xcode-select --install`，依畫面完成 [Apple Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools/) 安裝 |
| Ubuntu / Debian | `sudo apt update`，接著 `sudo apt install build-essential` |
| Fedora | `sudo dnf install gcc` |

已安裝編譯器就可直接執行啟動腳本。Windows 會先查 PATH 中的 GCC / Clang / MSVC，接著查 MSYS2 預設安裝位置與 Visual Studio Build Tools。缺少編譯器時會顯示對應提示。

需要指定編譯器時，`CC` 可以是可執行檔名稱或包含空白的完整路徑，例如：

```powershell
$env:CC = 'clang'
.\run.cmd
```

```sh
CC=clang sh ./run.sh
```

三個入口都支援遊戲參數，例如 Windows 的 `.\run.cmd --no-save --list` 或 Unix 的 `sh ./run.sh --no-save --list`。從其他資料夾用完整腳本路徑啟動也可以；存檔始終放在專案根目錄。

## 怎麼玩

```text
    # # # # #
    # . . . #
    # @ $ o #
    # . . . #
    # # # # #
```

`@` 是你，`$` 是箱子，`o` 是目標，`*` 是已放到目標的箱子，`+` 是站在目標上的玩家。把每個箱子推到目標就過關。只能推，不能拉。

| 操作 | 功能 |
|---|---|
| W / A / S / D + Enter | 上 / 左 / 下 / 右 |
| 多個移動字母 + Enter | 連續移動，例如 `ssdd` |
| U + Enter | 復原一步，最多保留最近 256 步 |
| R + Enter | 重玩本關 |
| M + Enter | 回到關卡選單 |
| H + Enter | 操作說明 |
| Q + Enter | 離開 |
| 過關後 N + Enter | 下一個尚未完成且已開啟的關卡 |

撞牆或推不動不計步。遊戲會提示卡進牆角的箱子；其他死路仍需要自行判斷，隨時可以復原或重玩。每關都有 Par（最佳步數目標），可以重玩挑戰。

完成紀錄及最佳步數保存在啟動時工作目錄的 `.git-game-progress`。`run.cmd` / `run.ps1` / `run.sh` 固定使用專案根目錄。這個存檔不會被 Git 追蹤。想從全新進度試玩，使用：

```powershell
.\run.cmd --no-save
```

## 關卡與解鎖題目

| 關卡 | 內容 | 對應題目 | 難度 |
|---|---|---|---|
| 1 First Patch | 一步推箱子的教學 | 初始開啟 | — |
| 2 First Commit | 繞到箱子後方的教學 | 初始開啟 | — |
| 3 Patch Warehouse | 雙箱平行配送 | [GPG-001](workshop/issues/GPG-001.md) | 一行，2–5 分鐘 |
| 4 Fork in the Road | 隔牆與中央通道 | [GPG-002](workshop/issues/GPG-002.md) | 一行，2–5 分鐘 |
| 5 Staging Area | 雙箱上推 | [GPG-003](workshop/issues/GPG-003.md) | 一行，2–5 分鐘 |
| 6 Perfect Commit | 從不同方向進入目標 | [GPG-004](workshop/issues/GPG-004.md) | 一行，2–5 分鐘 |
| 7 Merge Yard | 交會位置與配送順序 | [GPG-005](workshop/issues/GPG-005.md) | 一行，2–5 分鐘 |
| 8 Branch Detour | 換方向繞路推箱 | [GPG-006](workshop/issues/GPG-006.md) | 一行，2–5 分鐘 |
| 9 Release Vault | 三箱發布倉庫 | [GPG-007](workshop/issues/GPG-007.md) | 位元旗標，5–10 分鐘 |
| 10 Final Pull Request | 三箱與窄路 | [GPG-008](workshop/issues/GPG-008.md) | 啟用功能，2–5 分鐘 |
| 11 Cherry Pick | 雙箱路線謎題 | [GPG-009](workshop/issues/GPG-009.md) | 一行，1–3 分鐘 |
| 12 Stash Corner | 雙箱路線謎題 | [GPG-010](workshop/issues/GPG-010.md) | 一行，1–3 分鐘 |
| 13 Revert Route | 雙箱路線謎題 | [GPG-011](workshop/issues/GPG-011.md) | 一行，1–3 分鐘 |
| 14 Fetch Depot | 雙箱路線謎題 | [GPG-012](workshop/issues/GPG-012.md) | 一行，1–3 分鐘 |
| 15 Pull Station | 雙箱路線謎題 | [GPG-013](workshop/issues/GPG-013.md) | 一行，1–3 分鐘 |
| 16 Push Platform | 雙箱路線謎題 | [GPG-014](workshop/issues/GPG-014.md) | 一行，1–3 分鐘 |
| 17 Diff Alley | 雙箱路線謎題 | [GPG-015](workshop/issues/GPG-015.md) | 一行，1–3 分鐘 |
| 18 Log Lane | 雙箱路線謎題 | [GPG-016](workshop/issues/GPG-016.md) | 一行，1–3 分鐘 |
| 19 Status Square | 雙箱路線謎題 | [GPG-017](workshop/issues/GPG-017.md) | 一行，1–3 分鐘 |
| 20 Tag Terrace | 雙箱路線謎題 | [GPG-018](workshop/issues/GPG-018.md) | 一行，1–3 分鐘 |
| 21 Origin Outpost | 雙箱路線謎題 | [GPG-019](workshop/issues/GPG-019.md) | 一行，1–3 分鐘 |
| 22 Upstream Yard | 雙箱路線謎題 | [GPG-020](workshop/issues/GPG-020.md) | 一行，1–3 分鐘 |
| 23 Remote Room | 雙箱路線謎題 | [GPG-021](workshop/issues/GPG-021.md) | 一行，1–3 分鐘 |
| 24 Branch Garden | 雙箱路線謎題 | [GPG-022](workshop/issues/GPG-022.md) | 一行，1–3 分鐘 |
| 25 Squash Shed | 雙箱路線謎題 | [GPG-023](workshop/issues/GPG-023.md) | 一行，1–3 分鐘 |
| 26 Fast Forward | 雙箱路線謎題 | [GPG-024](workshop/issues/GPG-024.md) | 一行，1–3 分鐘 |
| 27 Checkout Crossing | 雙箱路線謎題 | [GPG-025](workshop/issues/GPG-025.md) | 一行，1–3 分鐘 |
| 28 Bisect Bridge | 雙箱路線謎題 | [GPG-026](workshop/issues/GPG-026.md) | 一行，1–3 分鐘 |
| 29 Reset Retreat | 雙箱路線謎題 | [GPG-027](workshop/issues/GPG-027.md) | 一行，1–3 分鐘 |
| 30 Release Party | 雙箱路線謎題 | [GPG-028](workshop/issues/GPG-028.md) | 一行，1–3 分鐘 |

28 個題目分散在 `src/unlocks/` 的 28 個小檔案，可以獨立認領，不必先修別人的題目。全部要求完成兩個教學；部分關卡另要求教學最佳步數達到 Par（第 1 關 1 步，第 2 關 4 步），以各 issue 的入場規格為準。

## 驗證自己的修改

先重新編譯，再用題目指定的關卡編號檢查。例如 GPG-001 解鎖第 3 關：

```powershell
.\scripts\build.ps1
.\build\git-game.exe --check-unlock 3
.\scripts\build.ps1 -Tests
```

Unix 對應指令：

```sh
sh scripts/build.sh
./build/git-game --check-unlock 3
sh scripts/build.sh --tests
```

`--check-unlock N` 使用「教學兩關已完成，最佳步數是 1 與 4」的固定測試紀錄，**不會修改存檔，也不會直接讓你遊玩尚未解鎖的關卡**。修好後應印出 `PASS: UNLOCKED` 並回傳 0。起始版本的 3–30 關會印出 `FAIL: STILL LOCKED`，這正是待修的問題。

基礎測試檢查三十張地圖的格式、推箱規則、復原、重玩、存檔，以及不能在教學未完成時提早解鎖。它不要求所有待解題目同時修好，所以只修一題也能通過 CI。

## GitHub 練習

完整操作見 [CONTRIBUTING.md](CONTRIBUTING.md)。流程是：

```text
認領 issue → fork → clone → branch → 修正 → 測試
→ add → commit → push → PR → review → merge → 同步 → 玩新關卡
```

題目代碼 `GPG-001` 和 GitHub issue 編號不同。PR 的 `Closes #...` 請使用本 repo 的實際編號。

## 跨平台檢查

GitHub Actions 設定了 Ubuntu、macOS、Windows Clang 與 Windows MSVC 四組檢查：

- 使用當地編譯器編譯並執行 C 規則測試。
- 從別的工作目錄呼叫啟動腳本。
- 專案路徑與指定編譯器路徑包含空白。
- 透過標準輸入完成兩個教學關卡。
- 確認參數、exit code、存檔位置與未修復題目都保留正確行為。
- 缺少指定編譯器時停止，顯示明確錯誤。

維護者可用 `node tests/test-launchers.mjs` 執行啟動檢查；Node 只用於這組維護測試與 CI，遊玩及 C 規則測試只需 C 編譯器。GitHub Actions 的跨平台結果在專案上傳並觸發 workflow 後查看。

## 原始碼導覽

```text
include/game.h          共用資料型別與函式宣告
src/main.c             選單、操作與命令列檢查
src/game.c             移動、推箱、復原與過關判定
src/levels.c           三十張地圖與最佳步數目標
src/progress.c         完成紀錄與本機存檔
src/render.c           ASCII 終端機畫面
src/unlocks/*.c        學員主要修改的 28 個小題目
run.cmd / run.ps1      Windows 啟動入口
run.sh                macOS / Linux / Git Bash 啟動入口
workshop/issues/       中文問題回報與功能需求
scripts/               編譯與測試
```

授權：MIT。
