import argparse
from datetime import datetime

def get_args():
    
    # TODO: Add --start-date, --end-date and --output arguments
    #       Convert the two dates to datetime objects
    
    parser = argparse.ArgumentParser()
    parser.add_argument('--startdate', type=str)
    parser.add_argument('--enddate', type=str)
    parser.add_argument('--output', type=str)
    args = parser.parse_args()

    start_date = args.startdate.split('-')
    end_date = args.enddate.split('-')
    args = list([start_date, end_date, args.output])
    return args



