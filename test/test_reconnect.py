import time
import os
import pexpect
import unittest
PROJECT_DIR = os.path.join(os.getcwd(), '..')
CLIENT_DIR = PROJECT_DIR + '/client/Mf'
SERVER_DIR = PROJECT_DIR + '/server/Mf'


class TestReconnect(unittest.TestCase):
	def setUp(self):
		#print('\n'+ self._testMethodName + ' ', end='')
		self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)

	def tearDown(self):
		self.server.terminate(True)
	def test_reconnect_before_login(self):	
		self.server.terminate(True)
		child = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
		try:
			child.expect('Waiting for server online', timeout = 1)
			self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
			child.expect('Enter username', timeout = 1)
			login_not_connect(child, 'admin', 'admin')
			child.expect('MENU', timeout = 1)
			result = True
		except Exception as err:
			result = False

		end_connection(child)
		self.assertTrue(result)
	def test_reconnect_login(self):
		child = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
		try:
			child.expect('Enter username', timeout = 1)
			child.sendline('admin')
			child.expect('Enter password', timeout = 1)
			self.server.terminate(True)
			child.sendline('admin')
			child.expect(['Server is not found', 'Enter username:'], timeout = 1)
			result = True
		except Exception as err:
			result = False
		end_connection(child)
		self.assertTrue(result)
	def test_reconnect_command_line(self):
		child = login('admin', 'admin')

		try:
			child.expect('Enter your command', timeout = 1)
			self.server.terminate(True)
			#child.expect('Err receiver package from server', timeout = 1)
			child.sendline('dummy_command')
			child.expect(['Command not found', 'Enter your command'], timeout = 1)
			self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
			time.sleep(1)
			child.sendline('?with test1')
			child.expect('@me<with>test1', timeout = 1)
			result = True
		except Exception as err:
			result = False
		end_connection(child)
		self.assertTrue(result)

	def test_reconnect_command_line_1(self):
		child = login('admin', 'admin')

		try:
			child.expect('Enter your command', timeout = 1)
			self.server.terminate(True)
			#child.expect('Err receiver package from server', timeout = 1)
			#print("pass")
			child.sendline('?with test1')
			child.expect(['Server is not found', 'Enter your command'], timeout = 1)
			self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
			time.sleep(1)
			child.sendline('?with test1')
			child.expect('@me<with>test1', timeout = 1)
			result = True
		except Exception as err:
			result = False
		end_connection(child)
		self.assertTrue(result)

	def test_reconnect_with(self):
		child = login('admin', 'admin')
		try:
			child.expect('Enter your command', timeout = 1)
			#child.expect('Err receiver package from server', timeout = 1)
			#print("pass")
			child.sendline('?with test1')
			child.expect('@me<with>test1', timeout = 1)
			self.server.terminate(True)
			child.sendline('Hello test1')
			child.expect(['Server is not found', '@me<with>test1'], timeout = 1)
			self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
			child.sendline('Hello test1')
			child.expect('@me<with>test1', timeout = 1)
			result = True
		except Exception as err:
			result = False
		end_connection(child)
		self.assertTrue(result)
	def test_reconnect_join(self):
		child = login('admin', 'admin')
		try:
			child.expect('Enter your command', timeout = 1)
			#child.expect('Err receiver package from server', timeout = 1)
			#print("pass")
			child.sendline('?join CBA')
			child.expect('@me<with>CBA', timeout = 1)
			self.server.terminate(True)
			child.sendline('Hello CBA')
			#print("pass")
			child.expect(['Server is not found', 'me<with>CBA'], timeout = 1)
			self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)
			child.sendline('Hello CBA')
			child.expect('@me<with>CBA', timeout = 1)
			result = True
		except Exception as err:
			result = False
		end_connection(child)
		self.assertTrue(result)
def login(username, password):
	child = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
	child.sendline(username)
	child.sendline(password)
	return child

def login_not_connect(child, username, password):
	child.sendline(username)
	child.sendline(password)

def end_connection(child):
	if child.terminate == False:
		child.terminate(True)

if __name__ == "__main__":
	unittest.main()