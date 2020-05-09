
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

class TestFindUser(unittest.TestCase):
	def setUp(self):
		#print('\n'+ self._testMethodName + ' ', end='')
		self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)

	def tearDown(self):
		self.server.terminate(True)
		#child = subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)
	def test_find_your_self(self):
		child_1 = login('admin', 'admin')
		child_2 = login('test1','123')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusers admin')
			child_1.expect('[admin*]', timeout = 1)
			child_2.expect('Enter your command: ', timeout = 1)
			child_2.sendline('?findusers test1')
			child_2.expect('[test1*]', timeout = 1)
			result = True

		except Exception as e:
			result = False

		end_connection(child_2)
		end_connection(child_1)
		self.assertTrue(result)
	def test_find_multi_user(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusers test1 admin')
			child_1.expect(['No users found!', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)

	def test_find_user_not_found(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusers xnxx')
			child_1.expect(['No users found!', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)

	def test_find_blank_username(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusers')
			child_1.expect(['No users found!', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_findin_inline_nospace(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusersnosapce')
			child_1.expect(['Command not found', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_find_two_question(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('??findusers')
			child_1.expect(['Command not found', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_find_user_ofline(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusers test1')
			child_1.expect(['[test1]', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_find_user_same_uName(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusers test')
			child_1.expect(['[test1 | test2 | test3]', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_find_all_users(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusers *')
			child_1.expect(['[admin* | test1 | test2 | test3]', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_find_all_user_2(self):
		child_1 = login('admin', 'admin')
		child_2 = login('test1','123')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?findusers *')
			child_1.expect(['[admin* | test1* | test2 | test3]', 'Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		end_connection(child_2)
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