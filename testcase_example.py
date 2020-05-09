'''Test feature login'''

import pexpect
import os
import test_print_result as pr
PROJECT_DIR = os.path.join(os.getcwd(), '..')
CLIENT_DIR = PROJECT_DIR + '/src/build/client'
ACCOUNT_DIR = PROJECT_DIR + '/src/server'


        
def load_account():
    account = []
    with open(ACCOUNT_DIR + '/acc.txt', 'r') as f: 
        for line in f.readlines():
            acc = line.rstrip().split(":")
            account.append(acc)
    return account

def test_valid_accounts(accounts):
    try:
        for account in accounts:
            if test_login(account) == False:
                pr.print_result_test(test_valid_accounts.__name__, 0)
                return False
    except Exception as e:
        print(e)
    pr.print_result_test(test_valid_accounts.__name__, 1)
    return True

def test_wrong_accounts():
    accounts = [['abasdc','123'],['gdf','213'],['hjdfh','123213'],['dsfg','djdty']]
    
    for account in accounts:
        if test_login(account) == True:
            pr.print_result_test(test_wrong_accounts.__name__, 0)
            return None
            pass
        pass
    pr.print_result_test(test_wrong_accounts.__name__, 1)
    pass

def test_invalid_accounts():
    accounts = [['%abc','123'],['3123','@3123'],['12312!','12412~~']]
    for account in accounts:
        if test_login(account) == True:
            pr.print_result_test(test_invalid_accounts.__name__, 0)
            return None
    pr.print_result_test(test_invalid_accounts.__name__, 1)
    pass

def test_except_3times_login():
    accounts = [['abgfdc','11233'],['deqwe','3123'],['12312','abc']]
    child = pexpect.spawn(CLIENT_DIR + '/client', cwd=CLIENT_DIR)
    try:
        child.expect(['Please enter your username and password:', 'Username: '],timeout=1)
        child.sendline(accounts[0][0])
        child.expect('Password: ', timeout=1)
        child.sendline(accounts[0][1])
        
        child.expect('Invalid User.', timeout=1)
        child.sendline(accounts[1][0])
        child.expect('Password: ', timeout=1)
        child.sendline(accounts[1][1])
        child.expect('Invalid User.', timeout=1)
        
        child.sendline(accounts[2][0])
        child.expect('Password: ', timeout=1)
        child.sendline(accounts[2][1])
        child.expect('Invalid User.', timeout=1)
        
        child.expect(pexpect.EOF, timeout=1)
        pr.print_result_test(test_except_3times_login.__name__, 1)
        
    except TimeoutError:
        pr.print_result_test(test_except_3times_login.__name__, 0)
        pass


def test_login(account):
    child = pexpect.spawn(CLIENT_DIR + '/client', cwd=CLIENT_DIR)
    result = False
    try:
        child.expect(['Please enter your username and password:', 'Username: '],timeout=1)
        child.sendline(account[0])
        child.expect('Password: ', timeout=1)
        child.sendline(account[1])
        child.expect('Login successfully.', timeout=1)
        result = True
    except Exception as error:
        result = False
    if child.terminate() == False:
        child.terminate(True) 
    return result


def main():
    accounts = load_account()
    test_valid_accounts(accounts)
    test_wrong_accounts()
    test_invalid_accounts()
    test_except_3times_login()
    pass
if __name__ == "__main__":
    main()
