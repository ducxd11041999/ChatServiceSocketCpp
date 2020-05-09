import os
import unittest
import HtmlTestRunner
loader = unittest.TestLoader()
PROJECT_DIR = os.path.join(os.getcwd(), '..')
TEST_DIR = PROJECT_DIR + '/test'
RESULT_DIR = TEST_DIR + '/result'
suite  = loader.discover(TEST_DIR)
testRunner=HtmlTestRunner.HTMLTestRunner(output=RESULT_DIR)
testRunner.run(suite)
#runner = unittest.TextTestRunner(verbosity = 2)
#runner.run(suite)

