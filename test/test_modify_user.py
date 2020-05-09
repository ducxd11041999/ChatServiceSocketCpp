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
test_username = 'test1'
test_password = 'new_pass'
test_role     = '2'
DB_PWD = 'root'
CMD = 'MYSQL_PWD=' + DB_PWD + ' mysql -u root test<proc_file'

class TestModifyUser(unittest.TestCase):

    def setUp(self):
        #print('\n'+ self._testMethodName + ' ', end='')
        self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
        test_username = 'test1'
        test_password = 'new_pass'
        test_role     = '2'

    def tearDown(self):
        self.server.terminate(True)
        subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)
        
    def test_user_not_allowed(self):
        child = login('test1', '123')
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect(['Command not found', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)


    def test_modify_user(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_role = '1'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect('Enter username : ', timeout = 1)
            child.sendline(test_username)
            child.expect('Enter new password of ' + test_username + ' : ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter new Role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['Modify successfully', 'Enter your command: '], timeout = 1)
            if test_login(test_username, test_password, 'Admin Functions') == True:
                result = True
            else:
                result = False
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result) 


    def test_modify_user_not_exist(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username = 'this_usr_dnt_exist'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect('Enter username : ', timeout = 1)
            child.sendline(test_username)
            child.expect('Enter new password of ' + test_username + ' : ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter new Role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['Username is not exist', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)  

    def test_modify_user_online(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        
        test_username = 'test1'
        test_password = '123'
        child_user = login(test_username, test_password)
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect('Enter username : ', timeout = 1)
            child.sendline(test_username)
            child.expect('Enter new password of ' + test_username + ' : ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter new Role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['User is online, You can not modify the information', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        end_connection(child_user)
        self.assertTrue(result) 

    def test_modify_username_size(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username = 'toooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooolong'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect('Enter username : ', timeout = 1)
            child.sendline(test_username)
            child.expect(['Out size of username ! Please enter again !', 'Enter username : '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_modify_password_size(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_password = 'toooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooolong'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect('Enter username : ', timeout = 1)
            child.sendline(test_username)
            child.expect('Enter new password of ' + test_username + ' : ', timeout = 1)
            child.sendline(test_password)
            child.expect(['Out size of password! Please enter again !', 'Enter new password of ' + test_username + ' : '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result) 

    def test_modify_role_int(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_role  = 'notinthihi'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect('Enter username : ', timeout = 1)
            child.sendline(test_username)
            child.expect('Enter new password of ' + test_username + ' : ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter new Role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['Please enter a number !', 'Enter new Role: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result) 

    def test_modify_username_null(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username  = ''
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect('Enter username : ', timeout = 1)
            child.sendline(test_username)
            child.expect('Enter new password of ' + test_username + ' : ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter new Role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['Username is not exist', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_modify_password_null(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_password  = ''
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyuser')
            child.expect('Enter username : ', timeout = 1)
            child.sendline(test_username)
            child.expect('Enter new password of ' + test_username + ' : ', timeout = 1)
            child.sendline(test_password)
            child.expect('Enter new Role: ', timeout = 1)
            child.sendline(test_role)
            child.expect(['Sorry, can\'t modify user because password is empty', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)


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
