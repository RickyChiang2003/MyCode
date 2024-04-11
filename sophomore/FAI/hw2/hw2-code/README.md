# HW2: Pacman AI

作業的題目介紹在 __[hw2_questionfile.pdf](https://drive.google.com/drive/folders/1EwWE3j7o7XhK4JLhBwmAWh0ly7IthWaa?usp=sharing)__ 

值得注意的是，這次的python版本比較舊，autograder.py中的imp package在python3.11之後的版本很可能不支援，如果執行時出現這方面的報錯，我有在雲端上放一個沒有 img package 的 autograder.py檔案( __[hw2_new_autograder.zip](https://drive.google.com/drive/folders/1EwWE3j7o7XhK4JLhBwmAWh0ly7IthWaa?usp=sharing)__ )，直接載下來解壓縮，然後把本來的 autograder.py 換成這個新的(或你也可以直接現在換)。


## Implement ~~TO 恩陞~~：
__multiAgents.py__ 是作業中唯一需要學生寫code的檔案(小精靈移動的AI)，TODO則是被6組的 __YOUR CODE HERE__ 和 __YOUR CODE END__ 包起來的部分，那6組分別在第22,80,175,218,271,313行，想練習(或荼毒別人)的時候將這些地方挖空就好。如果你想知道每個函式在幹麻也可以去看 ghostAgents.py (鬼鬼移動的AI)、pacman.py 和 game.py 的內容。

執行時可以直接複製questionfile的路徑。答案的寫法很多元，除了我原本的以外，可以透過在google查 "pacman multiAgents.py" 就查到別人的以前寫的AI導作業(題目根本一模一樣)，甚至chatgpt也能問到。不一定要參考我的作法(但這次我感覺自己寫得不錯XD)。

和hw1不太一樣的是，這次的作業內演算法並沒有超出課程範圍。雖然總共有五個區域要完成，但卻只用到了課堂上教的3組pseudocode：  __minimax演算法__ 、 __alpha-Beta pruning__ 、 __expectimax演算法__ ，Q1和Q5甚至根本沒用什麼演算法，學國小生思考如何在較好的state回傳較高的score即可。所以看完關於這三個演算法的影片，你就可以拿著pseudo code來看source code了，這次比較簡單。


## 看題時的小提醒
Q5會被跑在Q4上，Q4寫完才能寫Q5，並且Q5需參考117行的scoreEvaluationFunction。(下面是Q5簡介和理由)

117行的scoreEvaluationFunction做的事情是輸入一個state，並回傳這個state的score (score越高代表輸入的state對小精靈而言越有利)。

之所以scoreEvaluationFunction能被使用是因為MultiAgentSearchAgent在144行將evalFn呼叫為self.evaluationFunction，而下方的其他class都有引用MultiAgentSearchAgent，所以下方其他class都能將evalFn表示的function拿來用。換言之，透過更改142行的evalFn值，就可以自在的將self.evaluationFunction改成我們想要的function。

Q5要修改的是開在全域的betterEvaluationFunction，與117行的scoreEvaluationFunction很像，這兩個function都都是輸入一個state，並回傳這個state的score。因此Q5便是要思考如何透過目前能獲得的資料來讓state依照好壞得到高或低分的score，在執行 __python autograder.py -q q5__ 時其他py檔會自動將前面的evalFn值改成'betterEvaluationFunction'，並且依題目要求拿來執行Q4的ExpectimaxAgent。

因此Q5會被跑在Q4上，Q4沒寫完不能寫Q5，並且Q5除了參考117行的function，也要注意高score和低score間的差距不能太大(我測試的結果是100以上就很不好了)，不然會讓Q4的結果變得很不穩定。 
