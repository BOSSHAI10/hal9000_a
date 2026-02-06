import re

import lldb
from lldb.plugins.parsed_cmd import ParsedCommand

import hal_utils

class ListProcessesCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.add_option(short_option='f',
                        long_option='filter',
                        help='Filter regex for name',
                        default='.*',
                        value_type=lldb.eArgTypeName)

    def get_short_help(self):
        return 'List processes.'        

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        proc_list_ptr = hal_utils.get_debug_data('ProcessList')
        proc_list = hal_utils.get_value_from_address(hal_utils.LIST_ENTRY_TYPE, proc_list_ptr)
        name_pattern = re.compile(self.get_parser().filter)

        def list_callback(list_entry):
            proc = hal_utils.containing_record(list_entry, hal_utils.PROCESS_TYPE, 'NextProcess') 
            proc_addr = hal_utils.get_address_of_value(proc)
            proc_name = hal_utils.get_process_name(proc)
            if not name_pattern.search(proc_name):
                return
            thread_count = 0
            
            def count_callback(list_entry):
                nonlocal thread_count
                thread_count = thread_count + 1

            thread_list = hal_utils.get_field(proc, 'ThreadList')
            hal_utils.traverse_list(thread_list, count_callback)

            print(f'Process {hex(proc_addr)}: {proc_name}; Thread count: {thread_count}')

        print('Processes:')
        hal_utils.traverse_list(proc_list, list_callback)

class DumpProcessCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.make_argument_element(lldb.eArgTypeAddress, 'plain')

    def get_short_help(self):
        return "Dump process based on address."

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_ctx, result):
        try:
            addr = int(args_array, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return
        
        process = hal_utils.get_value_from_address(hal_utils.PROCESS_TYPE,
                                                    addr)
        thread_list = hal_utils.get_field(process, 'ThreadList')

        def list_callback(list_entry):
            thread = hal_utils.containing_record(list_entry, hal_utils.THREAD_TYPE, 'ProcessList') 
            thread_addr = hal_utils.get_address_of_value(thread)
            thread_name = hal_utils.get_thread_name(thread)
            print(f'Thread {hex(thread_addr)}: {thread_name}')

        print(f'Process: {hex(addr)}')
        print(process)
        print('Threads of the process:')
        hal_utils.traverse_list(thread_list, list_callback)
