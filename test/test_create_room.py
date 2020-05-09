
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
test_id = 'test_id'
test_capacity = '10' 

DB_PWD = 'root'
CMD = 'MYSQL_PWD=' + DB_PWD + ' mysql -u root test<proc_file'

class TestCreateRoom(unittest.TestCase):

    def setUp(self):
        #print('\n' + self._testMethodName + ' ', end='')
        #result = False
        self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
        test_id = 'test_id'
        test_capacity = '10' 

    def tearDown(self):
        self.server.terminate(True)
        subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)
        
    def test_user_not_allowed(self):
        child = login('test1', '123')
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createroom')
            child.expect(['Command not found', 'Enter your command: '], timeout = 1)
            result = True
        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_create_room(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        child_user = login('test1', '123')
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createroom')
            child.expect(['Create New Room', 'Room id: '], timeout = 1)
            child.sendline(test_id)
            child.expect('Capacity: ', timeout = 1)
            child.sendline(test_capacity)
            child.expect('You have created the room.', timeout = 1)
            child_user.sendline('?join '+test_id)
            child_user.expect(['You entered room <' + test_id + '>', '@me<with>' + test_id + ':'], timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        end_connection(child_user)
        self.assertTrue(result)  

    def test_create_room_exist(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_id = 'CBA'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createroom')
            child.expect(['Create New Room', 'Room id: '], timeout = 1)
            child.sendline(test_id)
            child.expect('Capacity: ', timeout = 1)
            child.sendline(test_capacity)
            child.expect('The roomID already existed!', timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_create_room_id_null(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_id =  ''

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createroom')
            child.expect(['Create New Room', 'Room id: '], timeout = 1)
            for i in range(10):
                child.sendline(test_id)
                child.expect('Room id: ', timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result) 

    def test_create_room_id_size(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        test_id = 'tooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooolong'
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createroom')
            child.expect(['Create New Room', 'Room id: '], timeout = 1)
            child.sendline(test_id)
            child.expect(['Out size of roomID ! Please enter again !', 'Room id: '], timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_create_room_capacity_invalid(self):
        child = login(ADMIN_USR, ADMIN_PWD)
        capacity = ['', ' ', 'asdasd', '-1', '0', '123njkas']
        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createroom')
            child.expect(['Create New Room', 'Room id: '], timeout = 1)
            child.sendline(test_id)
            child.expect('Capacity: ', timeout = 1)
            for c in capacity:
                child.sendline(c)
                child.expect(['invalid capacity!!', 'Capacity: '], timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result) 

    def test_create_ignore_room_id(self):
        child = login(ADMIN_USR, ADMIN_PWD)

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createroom')
            child.expect(['Create New Room', 'Room id: '], timeout = 1)
            child.sendline('?ignore')
            child.expect('Enter your command: ', timeout = 1)
            result = True

        except Exception as e:
            result = False

        end_connection(child)
        self.assertTrue(result)

    def test_create_ignore_capacity(self):
        child = login(ADMIN_USR, ADMIN_PWD)

        try:
            child.expect('Enter your command: ', timeout = 1)
            child.sendline('?createroom')
            child.expect(['Create New Room', 'Room id: '], timeout = 1)
            child.sendline(test_id)
            child.expect('Capacity: ', timeout = 1)
            child.sendline('?ignore')
            child.expect('Enter your command: ', timeout = 1)
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
