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
test_capacity = '100' 

DB_PWD = 'root'
CMD = 'MYSQL_PWD=' + DB_PWD + ' mysql -u root test<proc_file'

class TestModifyRoom(unittest.TestCase):

    def setUp(self):
        #print('\n'+ self._testMethodName + ' ', end='')
        #result = False
        self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
        test_id = 'CBA'
        test_capacity = '10' 

    def tearDown(self):
        self.server.terminate(True)
        subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)
        
    def test_user_not_allowed(self):
        child = login('test1', '123')
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyroom')
            child.expect(['Command not found', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_modify_room(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyroom')
            child.expect(['Enter Room Name :'], timeout = 1)
            child.sendline(test_id)
            child.expect('Enter new capacity: ', timeout = 1)
            child.sendline(test_capacity)
            child.expect(['Successfully modified room \'' + test_id + '\'!', 'Enter you command: '], timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)  

    def test_modify_room_id_not_exist(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_id = 'dummy_room_id'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyroom')
            child.expect(['Enter Room Name :'], timeout = 1)
            child.sendline(test_id)
            child.expect('Enter new capacity: ', timeout = 1)
            child.sendline(test_capacity)
            child.expect(['Room \'' + test_id + '\' does not exists!', 'Enter your command: '], timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_modify_room_id_null(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_id =  ''

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyroom')
            child.expect(['Enter Room Name : '], timeout = 1)
            for i in range(10):
                child.sendline(test_id)
                child.expect('Enter Room Name : ', timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result) 

    def test_modify_room_id_size(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_id = 'tooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooolong'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyroom')
            child.expect(['Enter Room Name : '], timeout = 1)
            child.sendline(test_id)
            child.expect(['Out size of roomID ! Please enter again !', 'Enter Room Name : '], timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_modify_room_capacity_invalid(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        capacity = ['asdas', ' akjsd nj 10', '', '  ']
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?modifyroom')
            child.expect(['Enter Room Name : '], timeout = 1)
            child.sendline(test_id)
            child.expect('Enter new capacity: ', timeout = 1)
            child.sendline(capacity[0])
            child.expect(['Please enter a number !', 'Enter new capacity: '], timeout = 1)
            child.sendline(capacity[1])
            child.expect(['Please enter a number !', 'Enter new capacity: '], timeout = 1)
            child.sendline(capacity[2])
            child.expect(['Please enter a number !', 'Enter new capacity: '], timeout = 1)
            child.sendline(capacity[3])
            child.expect(['Please enter a number !', 'Enter new capacity: '], timeout = 1)

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
