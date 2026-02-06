import lldb
from lldb.plugins.parsed_cmd import ParsedCommand

import hal_utils

class DumpVmmReservationSpaceCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.make_argument_element(lldb.eArgTypeAddress, 'plain')

    def get_short_help(self):
        return "Dump vmm reservation space based on address."

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_ctx, result):
        try:
            addr = int(args_array, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return

        vmm_res_space = hal_utils.get_value_from_address(hal_utils.VMM_RESERVATION_SPACE_TYPE,
                                                    addr)
        vmm_res_list = hal_utils.get_field_as_address(vmm_res_space, 'ReservationList')
        vmm_bitmap_start = hal_utils.get_field_as_address(vmm_res_space, 'BitmapAddressStart')

        vmm_state_free = hal_utils.get_enum_member_value_as_unsigned(hal_utils.VMM_RESERVATION_STATE_TYPE,
                                                                    'VmmReservationStateFree')
        vmm_state_used = hal_utils.get_enum_member_value_as_unsigned(hal_utils.VMM_RESERVATION_STATE_TYPE,
                                                                    'VmmReservationStateUsed')
        vmm_state_last = hal_utils.get_enum_member_value_as_unsigned(hal_utils.VMM_RESERVATION_STATE_TYPE,
                                                                    'VmmReservationStateLast')

        vmm_res_size = hal_utils.VMM_RESERVATION_TYPE.GetByteSize()

        print(f'VMM Reservation Space: {hex(addr)}')
        print(vmm_res_space)
        print('VMM Reservations:')

        curr_ptr = vmm_res_list
        curr = hal_utils.get_value_from_address(hal_utils.VMM_RESERVATION_TYPE,
                                                curr_ptr)
        while hal_utils.get_field_as_unsigned(curr, 'State') != vmm_state_last and curr_ptr < vmm_bitmap_start:
            start_va = hal_utils.get_field_as_address(curr, 'StartVa')
            size = hal_utils.get_field_as_unsigned(curr, 'Size')
            page_rights = hal_utils.get_field_as_unsigned(curr, 'PageRights')
            state = hal_utils.get_field_as_unsigned(curr, 'State')
            uncacheable = hal_utils.get_field_as_unsigned(curr, 'Uncacheable')
            file = hal_utils.get_field_as_address(curr, 'BackingFile')
            if state == vmm_state_used:
                print(f'VMM Reservation {hex(curr_ptr)}: StartVa: {hex(start_va)}, Size: {hex(size)}, PageRights: {page_rights}, Unchacheable: {uncacheable}, BackingFile: {hex(file)}')
            curr_ptr = curr_ptr + vmm_res_size
            curr = hal_utils.get_value_from_address(hal_utils.VMM_RESERVATION_TYPE,
                                                    curr_ptr)

class DumpVmmReservationCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        parser.make_argument_element(lldb.eArgTypeAddress, 'plain')

    def get_short_help(self):
        return "Dump vmm reservation based on address."

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_ctx, result):
        try:
            addr = int(args_array, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return

        vmm_res = hal_utils.get_value_from_address(hal_utils.VMM_RESERVATION_TYPE,
                                                   addr)

        start_va = hal_utils.get_field_as_unsigned(vmm_res, 'StartVa')
        size = hal_utils.get_field_as_unsigned(vmm_res, 'Size')
        page_rights = hal_utils.get_field_as_unsigned(vmm_res, 'PageRights')
        state = hal_utils.get_field_as_unsigned(vmm_res, 'State')
        uncacheable = hal_utils.get_field_as_unsigned(vmm_res, 'Uncacheable')
        file = hal_utils.get_field_as_address(vmm_res, 'BackingFile')

        bitmap = hal_utils.get_field(vmm_res, 'CommitBitmap')
        bitmap_buff = hal_utils.get_field_as_address(bitmap, 'BitmapBuffer')
        bitmap_buff_size = hal_utils.get_field_as_unsigned(bitmap, 'BufferSize')
        bitmap_bits = hal_utils.get_field_as_unsigned(bitmap, 'BitCount')
        
        print(f'VMM Reservation {hex(addr)}:')
        print(f'StartVa: {hex(start_va)}, Size: {hex(size)}, PageRights: {page_rights}, State: {state}, Unchacheable: {uncacheable}, BackingFile: {hex(file)}')
        print('Commit bitmap:')
        
        try:
            buff = bytearray(hal_utils.DEBUGGER_PROCESS.ReadMemory(bitmap_buff, bitmap_buff_size, lldb.SBError()))
            burst_start = 0 
            burst_val = buff[0] & 0x1

            def print_burst(st, en, val):
                start = start_va + st * 0x1000
                end = start_va + en * 0x1000
                if val == 0:
                    state = 'Reserved'
                else:
                    state = 'Commited'
                print(f'{hex(start)} - {hex(end)}: {state}') 

            for i in range(bitmap_bits):
                ndx = i // 8
                bit = i % 8
                curr_bit = (buff[ndx] >> bit) & 0x1
                if curr_bit != burst_val:
                    print_burst(burst_start, i - 1, burst_val)
                    burst_start = i
                    burst_val = curr_bit
            print_burst(burst_start, bitmap_bits - 1, burst_val)
        except Exception as e:
            print(e)
