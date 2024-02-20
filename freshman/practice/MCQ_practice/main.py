from mcq import *

test_name = input("標題: ")
cnt = int(input("題數: "))
test = [0 for i in range(1000)]
ans = [0 for i in range(1000)]

mode = input("輸入set以開始出題模式:")
if mode == "set":
    set_question(cnt, test)
do_question(cnt, test)
