from time import sleep
import requests
from lxml import etree

class Crawler(object):
    def __init__(self,
                 base_url='https://www.csie.ntu.edu.tw/news/',
                 rel_url='news.php?class=101'):
        self.base_url = base_url
        self.rel_url = rel_url
        self.url = 'https://www.csie.ntu.edu.tw/news/news.php?class=101'

    def crawl(self, start_date, end_date):

        response = requests.get("https://www.csie.ntu.edu.tw/news/news.php?class=101")
        html_text = response.content.decode()
        html_text[:50]
        '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 trans'
        root = etree.HTML(html_text)
        date = root.xpath("")

        """
        Main crawl API
        1. Note that you need to sleep 0.1 seconds for any request.
        2. It is welcome to modify TA's template.
        """

        contents = list()
        page_num = 0
        while True:
            rets, last_date = self.crawl_page(
                start_date, end_date, page=f'&no={page_num}')
            page_num += 10
            if rets:
                contents += rets
            if last_date < start_date:
                break
        return contents

    def crawl_page(self, start_date, end_date, page=''):


        """
        Parse ten rows of the given page
        Parameters:
            start_date (datetime): the start date (included)
            end_date (datetime): the end date (included)
            page (str): the relative url specified page num
        Returns:
            content (list): a list of date, title, and content
            last_date (datetime): the smallest date in the page
        """
        res = requests.get(
            self.base_url + self.rel_url + page,
            headers={'Accept-Language':
                     'zh-TW,zh;q=0.9,en-US;q=0.8,en;q=0.7,zh-CN;q=0.6'}
        ).content.decode()
        sleep(0.1)
        
        # TODO: parse the response and get dates, titles and relative url with etree
        
        ans = ""
        root = etree.HTML(res)
        content = root.xpath("//div[@class='editor content']")
        for i in content[0].itertext():
            ans += i
            # TODO: 1. concatenate relative url to full url
            #       2. for each url call self.crawl_content
            #          to crawl the content
            #       3. append the date, title and content to
            #          contents
        return ans

    def crawl_content(self, url):

        """Crawl the content of given url
        For example, if the url is
        https://www.csie.ntu.edu.tw/news/news.php?Sn=15216
        then you are to crawl contents of
        ``Title : �ڻPDeepMind��A.I.��s����, My A.I. Journey with DeepMind Date : 2019-12-27 2:20pm-3:30pm Location : R103, CSIE Speaker : ���h�ǳդh, DeepMind Hosted by : Prof. Shou-De Lin Abstract: �ڱN�P�P�ǭ̤��ɡA�ڳդh�Z��s��[�JDeepMind�ҰѻP��projects (AlphaGo, AlphaStar�PAlphaZero)�A�H�αq�ڭӤH�PDeepMind�������良��AI�o�i���i��C Biography: ���h��, Aja Huang �x�W�H�A��߻O�W�v�d�j�Ǹ�T�u�{��s�ҳդh�A�{��DeepMind Staff Research Scientist�C``
        """
        raise NotImplementedError
