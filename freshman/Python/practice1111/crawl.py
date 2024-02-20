import requests
import csv
import argparse
from time import sleep
from lxml import etree
import unicodecsv as csv
with open('example.csv', 'wb') as f:
    writer = csv.writer(f, encoding='utf-8')
    writer.writerow(('English', 'Japanese'))
    writer.writerow((u'Hello', u'\xa0/r/5/5/t/gj'))
"""
root = etree.HTML(res)
content = root.xpath("//div[@class='editor content']")
print(content)
result = ""
for elem in content[0].itertext():
    """