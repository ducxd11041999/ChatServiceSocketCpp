import os
import pexpect
import unittest


PROJECT_DIR = os.path.join(os.getcwd(), '..')
CLIENT_DIR = PROJECT_DIR + '/client/Mf'
SERVER_DIR = PROJECT_DIR + '/server/Mf'


class TestLogin(unittest.TestCase):

	def setUp(self):
		#print('\n'+ self._testMethodName + ' ', end='')
		self.server = pexpect.spawn(SERVER_DIR + '/server', cwd = SERVER_DIR)

	def tearDown(self):
		self.server.terminate(True)

	def test_login_success(self):
		username = 'admin'
		password = 'admin'
		self.assertTrue(test_login(username, password))

	def test_login_username_not_exist(self):
		username = 'admin1'
		password = 'admin1'
		self.assertTrue(test_login(username, password, "Username not exist!!"))

	def test_login_wrong_password(self):
		username = 'admin'
		password = 'admin1'
		self.assertTrue(test_login(username, password, "Wrong pass"))

	def test_outsize_username(self):
		username = 'adminnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn'
		child = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
		result = False
		try:
			#print ("pass")
			child.expect(['Connection established!', 'Enter username: '], timeout = 1)
			child.sendline(username)
			#print ("pass")
			child.expect('Out size of username ! Please enter again !', timeout = 1)
			#print ("pass")
			result = True

		except Exception as e:
			result = False
			pass

		if child.terminate() == False:
			child.terminate(True)
		self.assertTrue(result)

	def test_outsize_password(self):
		#print("run")
		username = 'admin'
		password = 'adminnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn'
		child = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
		result = False
		try:
			#print ("pass")
			child.expect(['Connection established!', 'Enter username: '], timeout = 1)
			child.sendline(username)
			#print ("pass")
			child.expect('Enter password: ', timeout = 1)
			child.sendline(password)
			child.expect('Out size of password! Please enter again !', timeout = 1)
			#print ("pass")
			result = True

		except Exception as e:
			result = False
			pass

		if child.terminate() == False:
			child.terminate(True)
		self.assertTrue(result)
		
	def test_already_login(self):
		child_1 = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
		username = 'test1'
		password = '123'
		child_2 = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
		try:	
			child_1.expect(['Connection established!', 'Enter username: '], timeout = 1)
			child_1.sendline(username)
			child_1.expect('Enter password: ', timeout = 1)
			child_1.sendline(password)
			child_1.expect("MENU", timeout = 1)
		
			
			child_2.expect(['Connection established!', 'Enter username: '], timeout = 1)
			child_2.sendline(username)
			child_2.expect('Enter password: ', timeout = 1)
			child_2.sendline(password)
			child_2.expect("Sorry your account is being login elsewhere !!",timeout = 1)
			
			result = True

		except Exception as e:
			result = False
			pass
		
		if child_1.terminate() == False and child_2.terminate() == False:
			child_1.terminate(True)
			child_2.terminate(True)

		self.assertTrue(result)

	def test_admin_login_success(self):
		username = 'admin'
		password = 'admin'
		self.assertTrue(test_login(username, password, "Admin Functions"))

def test_login(username, password, statusLogin="MENU"):

	child = pexpect.spawn(CLIENT_DIR + '/client', cwd = CLIENT_DIR)
	try:
		#print ("pass")
		child.expect(['Connection established!', 'Enter username: '], timeout = 1)
		child.sendline(username)
		#print ("pass")
		child.expect('Enter password: ', timeout = 1)
		child.sendline(password)
		#print ("pass")
		child.expect(statusLogin,timeout = 1)
		#print ("pass")
		result = True

	except Exception as e:
		result = False
		pass

	if child.terminate() == False:
		child.terminate(True)
	return result


if __name__ == "__main__":
	unittest.main()