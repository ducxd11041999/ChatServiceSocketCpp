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
test_password = '123'
DB_PWD = 'root'
CMD = 'MYSQL_PWD=' + DB_PWD + ' mysql -u root test<proc_file'

class TestDeleteUser(unittest.TestCase):

    def setUp(self):
        #print('\n'+ self._testMethodName + ' ', end='')
        self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
        test_username = 'test1'

    def tearDown(self):
        self.server.terminate(True)
        subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)
        
    def test_user_not_allowed(self):
        child = login('test1', '123')
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?deleteuser')
            child.expect(['Command not found', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)


    def test_delete_user(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?deleteuser')
            child.expect('Enter username: ', timeout = 1)
            child.sendline(test_username)
            child.expect(['Deleted successfully !', 'Enter your command: '], timeout = 1)
            if test_login(test_username, test_password) == False:
                result = True
            else:
                result = False
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result) 


    def test_delete_user_not_exist(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username = 'this_usr_dnt_exist'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?deleteuser')
            child.expect('Enter username: ', timeout = 1)
            child.sendline(test_username)
            child.expect(['Username is not exist', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)  

    def test_delete_user_online(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        
        test_username = 'test1'
        test_password = '123'
        child_user = login(test_username, test_password)
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?deleteuser')
            child.expect('Enter username: ', timeout = 1)
            child.sendline(test_username)
            child.expect(['Sorry user is online. You can\'t delete this user !', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        end_connection(child_user)
        self.assertTrue(result) 

    def test_delete_username_size(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_username = 'tooooooooooooooooooooooooooooooooooooooooooooooooooooolong'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?deleteuser')
            child.expect('Enter username: ', timeout = 1)
            child.sendline(test_username)
            child.expect(['Out size of username ! Please enter again !', 'Enter username: '], timeout = 1)
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
