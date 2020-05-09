
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

class TestJoin(unittest.TestCase):
	def setUp(self):
		#print('\n'+ self._testMethodName + ' ', end='')
		self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)

	def tearDown(self):
		self.server.terminate(True)
		#child = subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)

	def test_join_success(self):

		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?join CBA')
			child_1.expect('me<with>CBA: ', timeout = 1)

			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)



	def test_join_room_not_found(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?join qwerty')
			child_1.expect(['The room does not exist', 'Enter your command: '], timeout = 1)

			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)


	def test_join_no_room(self):
		child_1 = login('admin', 'admin')

		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?join')
			child_1.expect(['The room does not exist', 'Enter your command: '], timeout = 1)

			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)

	def test_join_full_room(self):
		child_1 = login('admin', 'admin')
		child_2 =  login('test1', '123')
		try:
			child_1.sendline("?join ALONE")
			child_1.expect('me<with>ALONE: ', timeout = 1)
			child_2.sendline("?join ALONE")
			child_2.expect(['The room reaches its capacity or you are already in the room','Enter your command:'], timeout = 1 )
			result = True
		except Exception as e:
			result = False
			pass
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