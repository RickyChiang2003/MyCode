import argparse
import re
import signal
import time
import subprocess
from subprocess import Popen, PIPE

parser = argparse.ArgumentParser()
parser.add_argument("input", help = "The file name of a command list for ./service")
parser.add_argument("-s", dest='silent', action="store_true")
args = parser.parse_args()

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

service_tree  = {"Manager": []}
name_to_info = dict() # info = (pid, num)
secret_to_name = dict()

def transform_output(xx):
    output_lines = [xx]
    trans_lines = []
    for line in output_lines:
        if "has been spawned" in line:
            match = re.search(r"pid: (?P<pid>\d+), secret: (?P<secret>\d+)", line)
            if match is not None:
                service_name, pid, secret = line.split()[0], match.group("pid"), match.group("secret")
                secret_to_name[secret] = service_name
                line = line.replace(secret, f"[{service_name}-secret]") \
                        .replace(pid, f"[{service_name}-pid]")
        elif "new secret" in line:
            secret = line.split()[-1]
            line = line.replace(secret, f"[{secret_to_name[secret]}-secret]")
        trans_lines.append(line)
    return trans_lines[0] # what the fuck is this coding style

def get_service_pid(output_line):
    match = re.search(r"(?P<service_name>[a-zA-Z0-9]+) has been spawned, pid: (?P<pid>\d+)", output_line)
    if match is not None:
        service_name, pid = match.group("service_name"), match.group("pid")
        name_to_info[service_name] = [pid, (3, 5)[service_name != "Manager"]]
        if service_name == "Manager":
            transform_output(output_line)

def kill_service(service_name):
    children = service_tree[service_name]
    for child in children:
        kill_service(child)
    name_to_info.pop(service_name)
    service_tree.pop(service_name)

def check_fd_num():
    is_correct = True
    for service_name, (pid, fd_num) in name_to_info.items():
        ls = Popen(["ls", f"/proc/{pid}/fd"], stdout = PIPE)
        ls_output = ls.communicate()[0].decode("ascii").split()
        fd_num_ls = len(ls_output)
        if fd_num_ls > fd_num and not args.silent:
            print(bcolors.FAIL + f"{service_name} has too many file descriptors!"
                  f"[ans, yours] = {[fd_num, fd_num_ls]}" + bcolors.ENDC)
            is_correct = False
        if fd_num_ls < fd_num and not args.silent:
            print(bcolors.OKGREEN + f"{service_name} has less then {fd_num} file descriptors?"
                  f"How do you do that? :O [ans, yours] = {[fd_num, fd_num_ls]}" + bcolors.ENDC)
    if is_correct and not args.silent:
        print(bcolors.OKCYAN + "The number of the fds is correct after the command" + bcolors.ENDC)

def handle_input_line(input_line):
    if "kill" in input_line:
        target_name = input_line.split()[1]
        if not target_name in name_to_info:
            return
        kill_service(target_name)
        for (service_name, children) in service_tree.items():
            if target_name in children:
                name_to_info[service_name][1] -= 2
                service_tree[service_name].remove(target_name)
                break
    elif "spawn" in input_line:
        parent_name, child_name = input_line.split()[1], input_line.split()[2]
        if not parent_name in name_to_info:
            return
        name_to_info[parent_name][1] += 2
        service_tree[parent_name].append(child_name)
        service_tree[child_name] = []


def handle_output_line(output_line):
    if "has been spawned" in output_line:
        get_service_pid(output_line)
   

Sum_Ting_Wong = False
def run_service(args):
    # input_name, is_ordered = args.input, args.is_ordered == "1"
    input_name = args.input
    manager = Popen(['./service', 'Manager'], bufsize = 0, stdout = PIPE, stdin = PIPE)
    checker = Popen(['./checker'], bufsize = 0, stdout = PIPE, stdin = PIPE)
    if manager.stdout is not None and checker.stdout is not None:
        get_service_pid(manager.stdout.readline().decode("ascii"))
        checker.stdout.readline().decode("ascii")
    cmd_to_delim = {
        "spawn": "has spawned",
        "kill": "are killed",
        "exchange": "have exchanged",
        "non": "exist",
    }
    input_lines = open(input_name, "r").readlines()
    for input_line in input_lines:
        if not args.silent:
            print(bcolors.HEADER + f"Execute command {input_line[:-1]}" + bcolors.ENDC)
        output_is_correct = True
        if manager.stdin is not None and checker.stdin is not None:
            manager.stdin.write(input_line.encode("ascii"))
            checker.stdin.write(input_line.encode("ascii"))
            target = input_line.split()[1]
            if target not in service_tree:
                delim = cmd_to_delim["non"]
            else:
                delim = cmd_to_delim[input_line.split()[0]]
            while True:
                if manager.stdout is not None and checker.stdout is not None:
                    output_line  = manager.stdout.readline().decode("ascii")
                    correct_line = checker.stdout.readline().decode("ascii")
                    transformed_line = transform_output(output_line);
                    if correct_line != transformed_line:
                        output_is_correct = False
                        if not args.silent:
                            print(bcolors.WARNING + "Raw output is  " + output_line[:-1] + bcolors.ENDC)
                            print(bcolors.WARNING + "My output is   " + correct_line[:-1] + bcolors.ENDC)
                            print(bcolors.WARNING + "Your output is " + transformed_line[:-1] + bcolors.ENDC)
                    
                    if delim != cmd_to_delim["non"]:
                        handle_output_line(output_line)
                    if delim in output_line:
                        break
        if output_is_correct:
            if not args.silent:
                print(bcolors.OKGREEN + "Output is correct for this command" + bcolors.ENDC)
        else:
            Sum_Ting_Wong = True
        handle_input_line(input_line)
        check_fd_num()
        if not args.silent:
            print()

print(bcolors.OKBLUE + "Running " + args.input + "..." + bcolors.ENDC)
run_service(args)

if Sum_Ting_Wong:
    print(bcolors.FAIL + "Didn't pass " + args.input + "." + bcolors.ENDC)
else:
    print(bcolors.OKGREEN + "Passed test " + args.input + "!" + bcolors.ENDC)
