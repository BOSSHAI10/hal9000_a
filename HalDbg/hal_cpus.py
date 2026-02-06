import lldb
from lldb.plugins.parsed_cmd import ParsedCommand

import hal_utils

class ListCpusCommand(ParsedCommand):
    def setup_command_definition(self):
        None

    def get_short_help(self):
        return 'List CPUs.'        

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        core_count = hal_utils.DEBUGGER_PROCESS.GetNumThreads()

        print('CPUs:')
        for i in range(core_count):
            core = hal_utils.DEBUGGER_PROCESS.GetThreadAtIndex(i)
            frame = core.GetSelectedFrame()
            cpu = hal_utils.get_current_cpu(frame)  
            cpu_addr = hal_utils.get_address_of_value(cpu)
            apic_id = hal_utils.get_field_as_unsigned(cpu, 'ApicId')
            logical_apic_id = hal_utils.get_field_as_unsigned(cpu, 'LogicalApicId')
            print(f'Core {i + 1}: PCPU {hex(cpu_addr)}: ApicId: {apic_id}, LogicalApicId: {logical_apic_id}')

class DumpCpuCommand(ParsedCommand): 
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.make_argument_element(lldb.eArgTypeAddress, 'plain')

    def get_short_help(self):
        return 'Dump CPU based on address.'        
    
    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_ctx, result):
        try:
            addr = int(args_array, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return

        cpu = hal_utils.get_value_from_address(hal_utils.PCPU_TYPE,
                                                    addr)

        print(f'CPU: {hex(addr)}')
        print(cpu)
