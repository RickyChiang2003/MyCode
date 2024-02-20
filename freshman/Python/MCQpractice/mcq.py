class Q:
    def __init__(self, question, answer):
        self.question = question
        self.answer = answer

def set_question(cnt, test):
    print("---命題模式---\n")
    for i in range(1, cnt + 1):
        question = input("Q" + str(i) + ".\n")
        answer = input("A" + ".\n")
        test[i] = Q(question, answer)

def do_question(cnt, test):
    print("---開始答題---\n" + test_name + "\n")
    score = 0
    for i in range(1, cnt + 1):
        a = input("Q" + str(i) + ".\n" + test[i].question + "\n")
        if a == test[i].answer:
            score += 1
    print("答對" + str(score) + "題, 共" + str(cnt) + "題\n")