---
title: '部落格建立: Github pages + Hexo'
date: 2024-01-16 22:37:34
categories: 所學
tags: 部落格
---

__部落格建立與簡介__

<!--more-->
## 前言

我對於 __Github__ 本來就相當熟悉，再加上以前曾經自學過一點 __HTML__ 、__CSS__ 和 __Javascript__ 的撰寫，語法理解上也不會太困難，因此建立部落格的工具我選擇了 __Github__ 的內建功能 __Github pages__ 和一個由台灣大大開發的靜態檔案產生器 __Hexo__ 。未來或許會再將它改成 __Github__ 推薦的 __[Jekyll](https://docs.github.com/en/pages/setting-up-a-github-pages-site-with-jekyll)__ 或是號稱跑速世界最快的 __[Hugo](https://gohugo.io/)__ 來玩玩看，但畢竟 __Hexo__ 的中文資源和主題還是比較契合一般架站的需求，又有幸滑到 __[@recode-0](https://www.youtube.com/@recode-0)__ 放在YT上簡單易懂的教學影片，直接就將我推進了 __Hexo__ 的坑。


## 實作


## 實作細節

在對 __Hexo__ 有點概念之後，來提一些實際撰寫需要注意的細節。

- 設定檔

    首先是在根目錄資料夾 __blog__ 下的 ___config.yml__ 檔案。顧名思義，部落格的基本資訊和配置設定通通都記錄在這個檔案裡，可以透過官網 __<https://hexo.io/zh-tw/docs/configuration.html>__ 一項項確認自己需要哪些功能。不過，這可以在 __deploy__ 和 __theme__ 都調整好之後再慢慢研究，畢竟大部分的功能可以在需要時再上網查找，這裡只先介紹幾個必要的功能(´･ω･`)
    p.s. 官網沒有提及，但使用臺灣繁中時應將 __language__ 定成 __zh-TW__ ，不然有些主題會噴怪輸出

- 使用 __deploy__ 發佈到網頁

    擴充套件中的 __deploy__ 讓我們可以在本地端直接透過指令將改好的內容發佈上 __github.io__ 的網頁。雖然這個選項在檔案中預設是空白的，但可以按照以下四個步驟來快速設定：
    
    1. 在撰寫 __Hexo__ 的根目錄(若如上所述便是 __blog__ 資料夾下)以終端機指令安裝套件：
    ``` shell
    $ npm install hexo-deployer-git --save
    ```
    
    2. 打開根目錄的 ___config.yml__ 檔案，然後將其中 __deploy__ 的內容設定如下：
    ``` yml
    deploy:
        type: git
        repo: https://github.com/username/username.github.io.git
        branch: master
    ##  type: 部署模式
    ##  repo: Github Repository連結
    ##  branch: 選擇分支，通常選擇主分支名。
    ```
    
    3. 以終端機指令將 __blog__ 的內容部署到網頁上(第一次執行會需要登入 __Github__ 帳號)：
    ``` shell
    $ hexo deploy
    ```
    因為需要在發佈前刪除舊檔並生成新檔，通常會與 __hexo clean__ 和 __hexo generate__ 指令成套使用。
    
    4. 進入個人頁面 __https://username.github.io__ ，並確認發佈狀況。但是，有時網頁會出現快取殘留問題。如果內容在發佈後沒更改，可以在重開頁面前以快捷鍵 __Ctrl+Shift+R__ 清除快取並適度等待。

- 客製化主題
    上面已經介紹過主題的更改方法了，建議大家去找些星星數多，有大量開源者貢獻的主題，以避免在加入喜歡的特殊設定時卡bug，非常搞心態。
    這裡我推薦的主題是 __NexT__ ，他有簡潔的版面配置、單純舒適的色調、在內建功能多的同時又有很高的可塑性，在網路上的資源也很多。如果有偷偷計畫寫些長文，害怕太過複雜的背景也容易干擾閱讀，用 __NexT__ 便能透過更改背景顏色來輕易地顯現出小說閱讀器的質感，內建的四種 __Scheme__ 和 __darkmode__ 等也都可以透過更改該主題資料夾下的 ___config.yml__ 設定來進行切換，功能齊全。

更改背景顏色、圖片

擴充套件的 __theme__ 在[@recode-0](https://www.youtube.com/@recode-0)的影片有詳細介紹，




## ~~蝦看不懂啦我眼睛好痠~~

如果上面的教學寫得不堪入眼真的讓你看都看不下去，那你可以差評然後去看這個影片。
__[@recode-0](https://www.youtube.com/@recode-0)__ 的影片： __[十分鐘小魔法 - 不需要寫程式也不用花錢，10 分鐘架設出專屬於自己的部落格，建立個人品牌 | 使用 Hexo + GitHub](https://www.youtube.com/watch?v=WJk765ztHUg)__
他有精簡的中文架站細流，還附帶debug方法和方便撰寫時使用的 __markdown__ 語法查詢工具，就算是單純想認識 __Hexo__ 也很推。而且影片中講述的都是屬於 __Git__ 和 __Hexo__ 的基本架構，不太會出現因版本差異造成的問題，只要一步步照著做就不會出事，可以看完影片再回頭查找自己想要的功能。