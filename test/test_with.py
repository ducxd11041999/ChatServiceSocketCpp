
import os
import pexpect
import unittest
PROJECT_DIR = os.path.join(os.getcwd(), '..')
CLIENT_DIR = PROJECT_DIR + '/client/Mf'
SERVER_DIR = PROJECT_DIR + '/server/Mf'

class TestWith(unittest.TestCase):
	def setUp(self):
		#print('\n'+ self._testMethodName + ' ', end='')
		#result = False
		self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)

	def tearDown(self):
		self.server.terminate(True)

	def test_with_success(self):

		child_1 = login('admin', 'admin')
		child_2 = login('test1', '123')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?with test1')
			child_1.expect('me<with>test1#: ', timeout = 1)
			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
		end_connection(child_2)

		self.assertTrue(result)


	def test_with_success_offline(self):
		child_1 = login('admin', 'admin')

		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?with test1')
			child_1.expect(['me<with>test1: ', 'Send offline msgs to username: test1'], timeout = 1)

			result = True

		except Exception as e:
			result = False

		end_connection(child_1)

		self.assertTrue(result)


	def test_with_user_not_found(self):
		child_1 = login('admin', 'admin')

		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?with qwerty')
			child_1.expect(['This user is not found', 'Enter your command: '], timeout = 1)

			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)


	def test_with_no_user(self):
		child_1 = login('admin', 'admin')

		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?with')
			child_1.expect(['This user is not found', 'Enter your command: '], timeout = 1)

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