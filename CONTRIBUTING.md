# 用一次小修正，開一個新關卡

先認領一個 issue，在 issue 留言，再由講師確認認領狀態。一次 PR 處理一題；其他題目繼續留給同學。

## 1. Fork 與 clone

在課堂 repo 按 Fork，然後 clone **自己的 fork**。下列 `YOUR_NAME`、`TEACHER`、`CLASS_REPO` 要換成實際值。

```sh
git clone https://github.com/YOUR_NAME/CLASS_REPO.git
cd CLASS_REPO
git remote add upstream https://github.com/TEACHER/CLASS_REPO.git
git remote -v
git switch -c fix/gpg-001-warehouse
```

`origin` 指向自己的 fork；`upstream` 指向講師的課堂 repo。

## 2. 重現、修改、遊玩

依 README 編譯、玩兩個教學關卡。先執行自己題目的 `--check-unlock N` 確認問題存在。編輯該題的 `src/unlocks/*.c`，重新編譯，確認檢查從 FAIL 變成 PASS，並進入新關卡遊玩。

請保留教學關卡的前置條件。把函式改成 `return 1` 會讓新玩家跳過教學，無法通過驗收。

Windows 基礎測試：

```powershell
.\scripts\build.ps1 -Tests
```

Unix 基礎測試：

```sh
sh scripts/build.sh --tests
```

## 3. Add、commit、push

以倉庫題為例，只加入自己修改的檔案：

```sh
git status
git diff
git add src/unlocks/warehouse.c
git diff --staged
git commit -m "fix: unlock warehouse after both tutorials"
git push -u origin fix/gpg-001-warehouse
```

不要提交執行檔、本機存檔或順手修掉其他同學的題目。

## 4. PR 與 review

建立 PR 時，base 是**講師的課堂 repo / main**，head 是**自己的 fork / 工作分支**。

說明問題、修改原因、解鎖的關卡及驗證方式。在 PR 描述加 `Closes #實際issue編號`。例如實際 issue 是 #12，就寫 `Closes #12`；題目代碼 GPG-001 不代表它一定是 #1。

請同學 review：檢查剛好達標的情況、未完成教學的情況，以及是否真的能玩新關卡。如果收到修改意見，在同一分支修改、commit、push，原 PR 會更新。

## 5. 合併後同步，玩大家開啟的關卡

講師合併後，在乾淨的 working tree 同步：

```sh
git switch main
git fetch upstream
git merge --ff-only upstream/main
git push origin main
git log --oneline --graph --all
```

重新編譯後，本機原有教學紀錄可繼續使用，大家已修好的關卡會開啟。

若 `--ff-only` 失敗，先查看 `git status` 與 commit graph，請講師協助；不要用 force push 覆蓋工作。
