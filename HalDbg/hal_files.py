import re

import lldb
from lldb.plugins.parsed_cmd import ParsedCommand

import hal_utils

class ListFileObjectsCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.add_option(short_option='f',
                        long_option='filter',
                        help='Filter regex for name',
                        default='.*',
                        value_type=lldb.eArgTypeName)

    def get_short_help(self):
        return 'List all file objects.'        

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        file_list_ptr = hal_utils.get_debug_data('FileList')
        file_list = hal_utils.get_value_from_address(hal_utils.LIST_ENTRY_TYPE, file_list_ptr)
        name_pattern = re.compile(self.get_parser().filter)

        def list_callback(list_entry):
            file = hal_utils.containing_record(list_entry, hal_utils.FILE_OBJECT_TYPE, 'AllList') 
            file_addr = hal_utils.get_address_of_value(file)
            try:
                file_name = hal_utils.get_file_name(file)
            except:
                file_name = None
            if not file_name:
                file_name = 'NULL'
            if not name_pattern.search(file_name):
                return
            print(f'File object {hex(file_addr)}: {file_name}')

        print('File objects:')
        hal_utils.traverse_list(file_list, list_callback)

class DumpFileObjectCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.make_argument_element(lldb.eArgTypeAddress, 'plain')

    def get_short_help(self):
        return "Dump file object based on address."

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_ctx, result):
        try:
            addr = int(args_array, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return

        file_obj = hal_utils.get_value_from_address(hal_utils.FILE_OBJECT_TYPE,
                                                    addr)

        print(f'File object: {hex(addr)}')
        print(file_obj)
