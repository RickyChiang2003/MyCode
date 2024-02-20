from mcq.py import *

test_name = input("標題: ")
cnt = int(input("題數: "))
test = [0 for i in range(1000)]
ans = [0 for i in range(1000)]

mode = input("模式:")
if mode == "set":
    set_question(cnt, test)
else:
    do_question(cnt, test)
