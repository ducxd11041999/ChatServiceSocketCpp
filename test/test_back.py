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

class TestBack(unittest.TestCase):
	def setUp(self):
		#print('\n'+ self._testMethodName + ' ', end='')
		#result = False
		self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)

	def tearDown(self):
		self.server.terminate(True)
		#printResult(result)

	def test_back_with_success(self):

		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?with test1')
			child_1.expect('me<with>test1: ', timeout = 1)
			child_1.sendline('?back')
			child_1.expect(['Enter your command:'], timeout = 1)
			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)

	def test_back_join_success(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?join CBA')
			child_1.expect('me<with>CBA', timeout = 1)
			child_1.sendline('?back')
			child_1.expect(['Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)


	def test_back_inline_1(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?join CBA')
			child_1.expect('me<with>CBA', timeout = 1)
			child_1.sendline('?backkkkkkkkkkkkkkkkggggg')
			child_1.expect(['Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_back_inline_2(self):
		child_1 = login('admin', 'admin')

		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?join CBA')
			child_1.expect('me<with>CBA', timeout = 1)
			child_1.sendline('are you ready ?back')
			child_1.expect(['Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_back_inline_3(self):
		child_1 = login('admin', 'admin')

		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?join CBA')
			child_1.expect('me<with>CBA', timeout = 1)
			child_1.sendline('are you ready ?back ok aaaaaaa')
			child_1.expect(['Enter your command:'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
		self.assertTrue(result)

	def test_back_in_command_line(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?back')
			child_1.expect(['Command not found', 'Enter your command:'], timeout = 1)
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
