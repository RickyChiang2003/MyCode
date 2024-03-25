# HW1: Search

## Implement:
Write your search algorithms in *search.py* and do not edit any other files, except for testing.

## To 恩陞：
作業的題目介紹在 __[這裡](https://www.csie.ntu.edu.tw/~stchen/teaching/fai24/hw1/hw1.html)__
但每題的範例題目其實不只這裡這些，因此被我特地分類在 __maps__ 資料夾裡的 ___p1__ 到 ___p4__， __single__, __corner__, __multi__ 則是按照難易度分的，分別為1點, 4點, 多點。

__search.py__ 是作業中唯一需要學生寫code的檔案，但如果你想知道每個函式在幹麻也可以去看其他三個的內容，執行時則可以看下面的 __Examples of how to run HW1__ 來改執行路徑。比較電的寫法在 __[這個連結](https://gitlab.com/BrianLii/fai-hw1?fbclid=IwAR1rVTSh_R1VL38yx-OXXHOlPf18OeqXysbYxlDnX1-wiwH4nqK1QyVx8FA)__ ，而我這裡的 __search.py__ 是由他的做法去掉class後簡化而來的，有點毒瘤，看看就好。

值得提醒的是，順序上建議你要先知道 __priority_queue__， __disjoint set with Union by rank + Path compression__ ，然後再去看一遍網路上 __kruskal's algorithm__ 的程式碼，最後再去看題目和 __search.py__，不然你會燒雞然後對這個資料夾按刪除。

如果你想看一下程式執行的是對是錯，__test.zip__ 有信睿爸爸寫好的judge，只要把 __search.py__ 放到資料夾下的 __run.sh__ 旁邊執行 __sh run.sh__ 就可以一次驗完所有的case，可以拿我的code值接下去玩玩看，順便感受一下信睿深不見底的電力(雖然有修NASA的人應該都要會寫但他有那個時間去寫這種東西真的很強)。

## Requirements:
```
python3
pygame
```
## Running:
The main file to run this homework is hw1.py:

```
usage: hw1.py [-h] [--method {bfs,astar,astar_corner,astar_multi,fast}] [--scale SCALE]
              [--fps FPS] [--human] [--save SAVE]
              filename
```

Examples of how to run HW1:
```
python hw1.py ./maps/single/bigMaze.txt --method fast > output
```
```
python hw1.py tinySearch.txt --scale 30 --fps 10 --human
```

For help run:
```
python hw1.py -h
```







Help Output:
```
HW1 Search

positional arguments:
  filename              path to maze file [REQUIRED]

optional arguments:
  -h, --help            show this help message and exit
  --method {bfs,astar,astar_corner,astar_multi,fast}
                        search method - default bfs
  --scale SCALE         scale - default: 20
  --fps FPS             fps for the display - default 30
  --human               flag for human playable - default False
  --save SAVE           save output to image file - default not saved
```
