import re

import lldb
from lldb.plugins.parsed_cmd import ParsedCommand

import hal_utils

class ListThreadsCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.add_option(short_option='f',
                        long_option='filter',
                        help='Filter regex for name',
                        default='.*',
                        value_type=lldb.eArgTypeName)

    def get_short_help(self):
        return 'List all threads.'        

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        thread_list_ptr = hal_utils.get_debug_data('ThreadList')
        thread_list = hal_utils.get_value_from_address(hal_utils.LIST_ENTRY_TYPE, thread_list_ptr)
        name_pattern = re.compile(self.get_parser().filter) 

        def list_callback(list_entry):
            thread = hal_utils.containing_record(list_entry, hal_utils.THREAD_TYPE, 'AllList') 
            thread_addr = hal_utils.get_address_of_value(thread)
            thread_name = hal_utils.get_thread_name(thread)
            if not name_pattern.search(thread_name):
                return 
            thread_state = hal_utils.get_field(thread, 'State')
            state = hal_utils.enum_field_to_string(hal_utils.THREAD_STATE_TYPE, thread_state)
            if state:
                state = state.replace('ThreadState', '')
            proc = hal_utils.get_process_from_thread(thread)
            proc_addr = hal_utils.get_address_of_value(proc)
            proc_name = hal_utils.get_process_name(proc)
            print(f'Thread {hex(thread_addr)}: {thread_name}; Process {hex(proc_addr)}: {proc_name}; State: {state}')

        print('Threads:')
        hal_utils.traverse_list(thread_list, list_callback)

class ListRunningThreadsCommand(ParsedCommand):
    def setup_command_definition(self):
        None

    def get_short_help(self):
        return 'List running threads.'        

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        core_count = hal_utils.DEBUGGER_PROCESS.GetNumThreads()

        print('Running threads:')
        for i in range(core_count):
            core = hal_utils.DEBUGGER_PROCESS.GetThreadAtIndex(i)
            frame = core.GetSelectedFrame()
            thread = hal_utils.get_current_thread(frame) 
            thread_addr = hal_utils.get_address_of_value(thread)
            thread_name = hal_utils.get_thread_name(thread)
            proc = hal_utils.get_process_from_thread(thread)
            proc_addr = hal_utils.get_address_of_value(proc)
            proc_name = hal_utils.get_process_name(proc)
            print(f'Core {i + 1}: Thread {hex(thread_addr)}: {thread_name}; Process {hex(proc_addr)}: {proc_name}')

class DumpThreadCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.make_argument_element(lldb.eArgTypeAddress, 'plain')

    def get_short_help(self):
        return 'Dump thread based on address.'        
    
    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_ctx, result):
        try:
            addr = int(args_array, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return

        thread = hal_utils.get_value_from_address(hal_utils.THREAD_TYPE,
                                                    addr)

        print(f'Thread: {hex(addr)}')
        print(thread)
