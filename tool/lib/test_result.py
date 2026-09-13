import os
from lib.path_util import *

class test_type:
    PLAIN = 'plain'
    MEMORY = 'memory'

class test_result:
    def __init__(self):
        dirs = ['test', 'context', 'data', 'out']
        make_dir_if_not_exists(dirs)
        self.__path = os.path.join(*dirs, 'test_result.csv')

    def __enter__(self):
        if not os.path.exists(self.__path):
            open(self.__path, 'w').close()

        file = open(self.__path, 'r+')
        content = file.read()
        lines = content.splitlines()

        header = 'test name,classic test log,classic test passed,memory test log,memory test passed'

        if len(lines) < 1:
            lines.append(header)
        elif lines[0] != header:
            lines[0] = header

        for i in range(len(lines)):
            lines[i] = lines[i].split(',')

        self.__file = file
        self.__lines = lines

        return self

    def __find_test_index(self, test_name: str) -> int:
        for i in range(1, len(self.__lines)):
            line = self.__lines[i]
            name = line[0]
            if name == test_name:
                return i
        return -1

    def __lines_to_text(self) -> None:
        text = ''
        for line in self.__lines:
            line_text = ''
            ignore_comma = True
            for cell in line:
                line_text += ('' if ignore_comma else ',') + cell
                ignore_comma = False
            text += line_text+'\n'
        return text

    def mark_test(self, test_name:str, test_type: str, log_path:str, passed: bool) -> None:
        index = self.__find_test_index(test_name)
        log_column = 0
        passed_column = 0
        if index < 0:
            self.__lines.append(['None','None','-','None','-'])
            index = len(self.__lines) - 1
        if test_type == 'plain':
            log_column = 1
            passed_column = 2
        elif test_type == 'memory':
            log_column = 3
            passed_column = 4
        else:
            raise Exception(f'Invalid test_type "{test_type}"!')
        line = self.__lines[index]
        line[0] = test_name
        line[log_column] = log_path
        line[passed_column] = '+' if passed else '-'
    
        text = self.__lines_to_text()
        self.__file.seek(0)
        self.__file.write(text)
    
    def __exit__(self, exception_type, exception_value, exception_traceback):
        self.__file.close()