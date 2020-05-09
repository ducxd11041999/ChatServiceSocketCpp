from test_login import test_login

import os
import pexpect
import unittest
import subprocess
PROJECT_DIR = os.path.join(os.getcwd(), '..')
CLIENT_DIR = PROJECT_DIR + '/client/Mf'
SERVER_DIR = PROJECT_DIR + '/server/Mf'
CONFIG_DIR = PROJECT_DIR + '/server/config'

ADMIN_USR = 'admin'
ADMIN_PWD = 'admin'
test_username = 'test_username'
test_password = 'test_password' 
test_role     = '2'
DB_PWD = 'root'
CMD = 'MYSQL_PWD=' + DB_PWD + ' mysql -u root test<proc_file'

class TestCreateUser(unittest.TestCase):

    def setUp(self):
        #print('\n'+ self._testMethodName + ' ', end='')
        #result = False
        self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
        test_username = 'test_username'
        test_password = 'test_password' 
        test_role     = '2'

    def tearDown(self):
        self.server.terminate(True)
        subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)
        
    def test_user_not_allowed(self):
        child = login('test1', '123')
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Command not found', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

    def test_create_user(self):
        child = login(ADMIN_USR, ADMIN_PWD)

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter the role: ', timeout = 1)
            child.sendline(test_role)
            child.expect('Create success', timeout = 1)
            if test_login(test_username, test_password) == True:
                result = True
            else:
                result = False
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)  

    def test_create_admin(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_role = '1'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter the role: ', timeout = 1)
            child.sendline(test_role)
            child.expect('Create success', timeout = 1)
            if test_login(test_username, test_password, 'Admin Functions') == True:
                result = True
            else:
                result = False
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

    def test_create_username_exist(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username = 'test1'

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter the role: ', timeout = 1)
            child.sendline(test_role)
            child.expect('You can\'t create this user', timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result) 

    def test_create_username_size(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username = 'qwertyuiopasdfghjklzxcvbnmqwertyuiopoasdfghjklzxcvbnmqwertyuiop'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect(['Out size of username ! Please enter again !', 'Enter username: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result) 

    def test_create_password_size(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_password = 'nasdkbcasbcjasdhbcjahsdbchjasdbcjhasdbcjashdbckjasdbchakjsbchjasdh'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect(['Out size of password! Please enter again !', 'Enter password: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result) 

    def test_create_role_not_int(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_role = 'asdasd'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline('test1')
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter the role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['Please enter a number !', 'Enter the role: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result) 

    def test_create_username_null(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username = ''
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter the role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['Sorry. User name or pass word is null', 'Enter username: ', 'Create new user'], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

    def test_create_username_space(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username = 'haha hihi'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect(['Can not use space in your username! Please enter again!', 'Enter username: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

    def test_create_password_space(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_password = '12 3'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect(['Can not use space in your password! Please enter again!', 'Enter password: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

    def test_create_password_null(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_password = ''
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter the role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['Sorry. User name or pass word is null', 'Enter username: ', 'Create new user'], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

    def test_create_ignore_username(self):
        child = login(ADMIN_USR, ADMIN_PWD)

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline('?ignore')
            child.expect('Enter your command: ', timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

    def test_create_ignore_password(self):
        child = login(ADMIN_USR, ADMIN_PWD)

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline('?ignore')
            child.expect('Enter your command: ', timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

    def test_create_ignore_role(self):
        child = login(ADMIN_USR, ADMIN_PWD)

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createuser')
            child.expect(['Create new user', 'Enter username: '], timeout = 1)
            child.sendline(test_username)
            child.expect('Enter password: ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter the role: ', timeout = 1)
            child.sendline('?ignore')
            child.expect('Enter your command: ', timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        #self.assertTrue(result)

def login(username, password):
    child = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
    child.sendline(username)
    child.sendline(password)
    return child

def end_connection(child):
    if child.terminate == False:
        child.terminate(True)

if __name__ == "__main__":
	unittest.main()