import os
import subprocess
import signal
import pexpect
import unittest
PROJECT_DIR = os.path.join(os.getcwd(), '..')
CLIENT_DIR = PROJECT_DIR + '/client/Mf'
SERVER_DIR = PROJECT_DIR + '/server/Mf'
#CONFIG_DIR = PROJECT_DIR + '/server/config'
#DB_PASSWORD = '1'
#CMD = 'mysql -uroot -p' + DB_PASSWORD + ' test<proc_file'

class TestShowUsers(unittest.TestCase):
	def setUp(self):
		#print('\n'+ self._testMethodName + ' ', end='')
		self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)

	def tearDown(self):
		self.server.terminate(True)
		#child = subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)

	def test_show_one_users_as_admin(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?showusers')
			child_1.expect('[admin]', timeout = 1)
			
			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)
	def test_show_two_users_as_admin(self):
		child_1 = login('admin', 'admin')
		child_2 = login('test1', '123')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?showusers')
			child_1.expect('[admin|test1]', timeout = 1)
			result = True

		except Exception as e:
			result = False

		end_connection(child_2)
		end_connection(child_1)
		self.assertTrue(result)
	def test_show_user_ofline_after_online(self):
		child_1 = login('admin', 'admin')
		child_2 = login('test1', '123')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?showusers')
			child_1.expect('[admin|test1]', timeout = 1)
			end_connection(child_2)
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?showusers')
			child_1.expect('[admin]', timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_show_user_line_space(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?showusers allofuser') #case of show users you can change ?showuser + anything
			child_1.expect('[admin]', timeout = 1)	
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)
	def test_show_user_line_no_space(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?showusersallofuser') #case of show users you can change ?showuser + anything
			child_1.expect('Command not found', timeout = 1)	
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
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