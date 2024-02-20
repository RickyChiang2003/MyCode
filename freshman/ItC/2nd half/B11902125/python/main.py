import requests
import csv
import argparse
from time import sleep
from lxml import etree

#input
parser = argparse.ArgumentParser()
parser.add_argument('--start-date', dest = "startdate", type=str)
parser.add_argument('--end-date', dest = "enddate", type=str)
parser.add_argument('--output', dest = "output", type=str)
args = parser.parse_args()

#main process
res = requests.get("https://www.csie.ntu.edu.tw/news/news.php?class=101")
sleep(0.1)
res.encoding='utf-8'
html_text = res.content.decode()
html_text[:50]
'<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 trans'
root = etree.HTML(html_text)
addr1 = "//div[1]/div/div[2]/div/div/div[2]/div/table/tbody/tr["
addr2 = "]/td["

B = [range(3) for _ in range(100)]
B[0] = ["Post Date", "Title", "Content"]
last_i = 0
for i in range(1,99):
    str_i = str(i)
    S = addr1 + str_i + addr2
    dates = root.xpath(S + "1]")
    titles = root.xpath(S + "2]/a")
    list_dates = [date.text for date in dates]
    list_titles = [title.text for title in titles]
    if len(list_dates) == 0:
        #print("break")
        break
    elif (args.startdate > list_dates[0]) or (args.enddate < list_dates[0]):
        #print("continue")
        continue

    cs = root.xpath(S + "2]/a/@href")
    list_cs = [c for c in cs]
    list_cs[0] = str(list_cs[0])
    resc = requests.get("https://www.csie.ntu.edu.tw/news/" + list_cs[0])
    sleep(0.1)
    resc.encoding='utf-8'
    html_textc = resc.content.decode()
    html_textc[:50]
    '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 trans'
    rootc = etree.HTML(html_textc)    
    contents = rootc.xpath("//div[1]/div/div[2]/div/div/div[2]/div/div[2]//text()")
    ans = ""
    for k in contents:
        ans += k
    ans = ans.replace('\xa0','')
    B[i] = list_dates[0], list_titles[0], ans
    last_i = i
    #print(B[i], i)
    #print(type(list_dates[0]), type(B[i][2]))

with open(args.output, 'w', newline='', encoding='utf-8', errors='ignore') as csvfile:
  writer = csv.writer(csvfile)
  for i in range(0,(last_i + 1)):
    writer.writerow(B[i])


