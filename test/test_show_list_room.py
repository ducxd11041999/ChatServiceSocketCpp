
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

class TestShowListRoom(unittest.TestCase):
	def setUp(self):
		#print('\n'+ self._testMethodName + ' ', end='')
		self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)

	def tearDown(self):
		self.server.terminate(True)
		#child = subprocess.call(CMD, cwd = CONFIG_DIR, shell = True)

	def test_list_success(self):

		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?listroom')
			child_1.expect(['RoomID', 'Capacity:'], timeout = 1)

			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
		self.assertTrue(result)

	def test_list_room_line_no_space(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?listroommmmmm')
			child_1.expect(['Command not found'], timeout = 1)
			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
	def test_list_room_line_space(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('?listroom mmmmm')
			child_1.expect(['RoomID', 'Capacity:'], timeout = 1)
			result = True

		except Exception as e:
			result = False

		end_connection(child_1)
	def test_before_list_room(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('list ?listroom')
			child_1.expect(['Command not found'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)

	def test_two_question_list_room(self):
		child_1 = login('admin', 'admin')
		try:
			child_1.expect('Enter your command: ', timeout = 1)
			child_1.sendline('??listroom')
			child_1.expect(['Command not found'], timeout = 1)
			result = True
		except Exception as e:
			result = False
		end_connection(child_1)
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