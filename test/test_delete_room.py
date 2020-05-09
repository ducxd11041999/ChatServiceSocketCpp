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
test_id = 'CBA'

DB_PWD = 'root'
CMD = 'MYSQL_PWD=' + DB_PWD + ' mysql -u root test<proc_file'

class TestDeleteRoom(unittest.TestCase):

    def setUp(self):
        #print('\n'+ self._testMethodName + ' ', end='')
        self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
        test_id = 'CBA'

    def tearDown(self):
        self.server.terminate(True)
        subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)
        
    def test_user_not_allowed(self):
        child = login('test1', '123')
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?deleteroom')
            child.expect(['Command not found', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_delete_room(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        child_user = login('test1', '123')
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?deleteroom')
            child.expect(['Delete Room', 'Enter Room ID: '], timeout = 1)
            child.sendline(test_id)
            child.expect('Successfully deleted room \'' + test_id + '\'!', timeout = 1)
            child_user.sendline('?join '+test_id)
            child_user.expect(['The room does not exist.', 'Enter your command: '], timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        end_connection(child_user)
        self.assertTrue(result)  

    def test_delete_room_not_empty(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        child_user = login('test1', '123')
        test_id = 'CBA'
        try:
            child_user.sendline('?join '+test_id)
            child_user.expect(['You entered room <' + test_id + '>', '@me<with>' + test_id + ':'], timeout = 1)

            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?deleteroom')
            child.expect(['Delete Room', 'Enter Room ID: '], timeout = 1)
            child.sendline(test_id)
            child.expect(['There are users currently in room \'' + test_id + '\'!', 'Enter your command: '], timeout = 1)
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
