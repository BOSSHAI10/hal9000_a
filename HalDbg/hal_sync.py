import re
import inspect
import ctypes

import lldb
from lldb.plugins.parsed_cmd import ParsedCommand
from enum import Enum

import hal_utils
from hal_utils import FrameLocation

class LockType(Enum):
    Spinlock = 1,
    RwSpinlock = 3,

class ListMutexesCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()

        parser.add_option(short_option='f',
                        long_option='filter',
                        help='Filter regex for name',
                        default='.*',
                        value_type=lldb.eArgTypeName)

    def get_short_help(self):
        return 'List all mutexes.'        

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        mutex_list_ptr = hal_utils.get_debug_data('MutexList')
        mutex_list = hal_utils.get_value_from_address(hal_utils.LIST_ENTRY_TYPE, mutex_list_ptr)
        name_pattern = re.compile(self.get_parser().filter)
        
        def list_callback(list_entry):
            mutex = hal_utils.containing_record(list_entry, hal_utils.MUTEX_TYPE, 'AllList') 
            mutex_addr = hal_utils.get_address_of_value(mutex)
            mutex_name = hal_utils.get_name(mutex)
            if not mutex_name:
                mutex_name = 'NULL'
            if not name_pattern.search(mutex_name):
                return
            holder = hal_utils.get_field_as_address(mutex, 'Holder')
            print(f'Mutex {hex(mutex_addr)}: {mutex_name}')
            if holder != 0:
                holder_addr = holder
                holder = hal_utils.get_value_from_address(hal_utils.THREAD_TYPE, holder)
                holder_name = hal_utils.get_thread_name(holder)
                holder_proc = hal_utils.get_process_from_thread(holder)
                holder_proc_addr = hal_utils.get_address_of_value(holder_proc)
                holder_proc_name = hal_utils.get_process_name(holder_proc)
                print(f'  Holder: Thread {hex(holder_addr)}: {holder_name}; Process {hex(holder_proc_addr)}: {holder_proc_name}')

        print('Mutexes:')
        hal_utils.traverse_list(mutex_list, list_callback)

class DumpMutexCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        args = [parser.make_argument_element(lldb.eArgTypeAddress, repeat='plain')]
        parser.add_argument_set(args)
        parser.add_option(short_option='v',
                          long_option='verbose',
                          help='Verbose output',
                          default=False,
                          value_type=lldb.eArgTypeBoolean)

    def get_short_help(self):
        return "Dump mutex based on address."

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_ctx, result):
        buff = ctypes.create_string_buffer(b'\0' * 256)
        addr = args_array.GetItemAtIndex(0).GetStringValue(ctypes.addressof(buff))

        try:
            addr = int(addr, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return

        mutex = hal_utils.get_value_from_address(hal_utils.MUTEX_TYPE,
                                                    addr)
        waiting_list = hal_utils.get_field(mutex, 'WaitingList')
        holder = hal_utils.get_field_as_address(mutex, 'Holder')
      
        def list_callback(list_entry):
            thread = hal_utils.containing_record(list_entry, hal_utils.THREAD_TYPE, 'ReadyList') 
            thread_addr = hal_utils.get_address_of_value(thread)
            thread_name = hal_utils.get_thread_name(thread)
            proc = hal_utils.get_process_from_thread(thread)
            proc_addr = hal_utils.get_address_of_value(proc)
            proc_name = hal_utils.get_process_name(proc)
            print(f'Thread {hex(thread_addr)}: {thread_name}; Process {hex(proc_addr)}: {proc_name}')

        print(f'Mutex: {hex(addr)}')
        if self.get_parser().verbose:
            print(mutex)
        if holder != 0:
            holder_addr = holder
            holder = hal_utils.get_value_from_address(hal_utils.THREAD_TYPE, holder)
            holder_name = hal_utils.get_thread_name(holder)
            holder_proc = hal_utils.get_process_from_thread(holder)
            holder_proc_addr = hal_utils.get_address_of_value(holder_proc)
            holder_proc_name = hal_utils.get_process_name(holder_proc)
            print(f'Holder: Thread {hex(holder_addr)}: {holder_name}; Process {hex(holder_proc_addr)}: {holder_proc_name}')
        else:
            print('Holder: NULL')
        print(f'Waiting list: ')
        hal_utils.traverse_list(waiting_list, list_callback)

class ListExEventsCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()

        parser.add_option(short_option='f',
                        long_option='filter',
                        help='Filter regex for name',
                        default='.*',
                        value_type=lldb.eArgTypeName)

    def get_short_help(self):
        return 'List all executive events.'        

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        ex_event_list_ptr = hal_utils.get_debug_data('ExEventList')
        ex_event_list = hal_utils.get_value_from_address(hal_utils.LIST_ENTRY_TYPE, ex_event_list_ptr)
        name_pattern = re.compile(self.get_parser().filter)

        def list_callback(list_entry):
            event = hal_utils.containing_record(list_entry, hal_utils.EX_EVENT_TYPE, 'AllList')
            event_addr = hal_utils.get_address_of_value(event)
            event_name = hal_utils.get_name(event)
            if not event_name:
                event_name = 'NULL'
            if not name_pattern.search(event_name):
                return
            print(f'EX Event {hex(event_addr)}: {event_name}')

        print('EX Events:')
        hal_utils.traverse_list(ex_event_list, list_callback) 

class DumpExEventCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        args = [parser.make_argument_element(lldb.eArgTypeAddress, repeat='plain')]
        parser.add_argument_set(args)
        parser.add_option(short_option='v',
                          long_option='verbose',
                          help='Verbose output',
                          default=False,
                          value_type=lldb.eArgTypeBoolean)

    def get_short_help(self):
        return "Dump ex event based on address."

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_ctx, result):
        buff = ctypes.create_string_buffer(b'\0' * 256)
        addr = args_array.GetItemAtIndex(0).GetStringValue(ctypes.addressof(buff))

        try:
            addr = int(addr, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return

        ex_event = hal_utils.get_value_from_address(hal_utils.EX_EVENT_TYPE,
                                                    addr)
        waiting_list = hal_utils.get_field(ex_event, 'WaitingList')

        def list_callback(list_entry):
            thread = hal_utils.containing_record(list_entry, hal_utils.THREAD_TYPE, 'ReadyList') 
            thread_addr = hal_utils.get_address_of_value(thread)
            thread_name = hal_utils.get_thread_name(thread)
            proc = hal_utils.get_process_from_thread(thread)
            proc_addr = hal_utils.get_address_of_value(proc)
            proc_name = hal_utils.get_process_name(proc)
            print(f'Thread {hex(thread_addr)}: {thread_name}; Process {hex(proc_addr)}: {proc_name}')

        print(f'EX Event: {hex(addr)}')
        if self.get_parser().verbose:
            print(ex_event)
        print(f'Waiting list: ')
        hal_utils.traverse_list(waiting_list, list_callback)

class ListLocksCommand(ParsedCommand):

    def setup_command_definition(self):
        enum_values = [
            ['Spinlock', 'Select all spinlocks'],
            ['RwSpinlock', 'Select all rw spinlocks'],
        ]

        parser = self.get_parser()
        parser.add_option(short_option='t',
                        long_option='type',
                        help='Filters displayed locks based on type',
                        default='Spinlock',
                        value_type=lldb.eArgTypeTypeName,
                        enum_values=enum_values)
        parser.add_option(short_option='f',
                        long_option='filter',
                        help='Filter regex for name',
                        default='.*',
                        value_type=lldb.eArgTypeName)

    def get_short_help(self):
        return 'List all locks of a given type.'        

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        name_pattern = re.compile(self.get_parser().filter) 

        requested_type = LockType[self.get_parser().type]

        if requested_type == LockType.RwSpinlock:
            lock_list_ptr = hal_utils.get_debug_data('RwSpinlockList')
        else:
            lock_list_ptr = hal_utils.get_debug_data('LockList')
        
        lock_list = hal_utils.get_value_from_address(hal_utils.LIST_ENTRY_TYPE, lock_list_ptr)

        def list_callback(list_entry):
            if requested_type == LockType.RwSpinlock:
                lock = hal_utils.containing_record(list_entry, hal_utils.RW_SPINLOCK_TYPE, 'AllList')
            elif requested_type == LockType.Spinlock:
                lock = hal_utils.containing_record(list_entry, hal_utils.SPINLOCK_TYPE, 'AllList')
            lock_name = hal_utils.get_name(lock)
            lock_addr = hal_utils.get_address_of_value(lock)
            if not lock_name:
                lock_name = 'NULL'
            if not name_pattern.search(lock_name):
                return
            print(f'Lock {hex(lock_addr)}: {lock_name}')

        print('Locks:')
        hal_utils.traverse_list(lock_list, list_callback)

class DumpSpinlockCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        args = [parser.make_argument_element(lldb.eArgTypeAddress, repeat='plain')]
        parser.add_argument_set(args)
        parser.add_option(short_option='v',
                          long_option='verbose',
                          help='Verbose output',
                          default=False,
                          value_type=lldb.eArgTypeBoolean)

    def get_short_help(self):
        return "Dump spinlock based on address."

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused

    def _in_lock_acquire(self, func_name):
        if func_name == 'SpinlockAcquire':
            return True

        if func_name == 'MonitorLockAcquire':
            return True
        
        return False

    def __call__(self, debugger, args_array, exe_ctx, result):
        buff = ctypes.create_string_buffer(b'\0' * 256)
        addr = args_array.GetItemAtIndex(0).GetStringValue(ctypes.addressof(buff))

        try:
            addr = int(addr, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return

        lock = hal_utils.get_value_from_address(hal_utils.SPINLOCK_TYPE, addr)
        lock_holder = hal_utils.get_field_as_unsigned(lock, 'Holder')
        print(f'Lock {hex(addr)}:')
        print(f'Holder: {hex(lock_holder)}')
        if self.get_parser().verbose:
            print(lock)
        print('Waiters:')
        
        core_count = hal_utils.DEBUGGER_PROCESS.GetNumThreads()
        for i in range(core_count):
            core = hal_utils.DEBUGGER_PROCESS.GetThreadAtIndex(i)
            frame = core.GetSelectedFrame()
            if self._in_lock_acquire(frame.GetDisplayFunctionName()):
                spinning = frame.FindVariable('spinning')
                if not spinning:
                    continue
                lock_addr = frame.FindVariable('Lock').GetValueAsAddress()
                if not lock_addr:
                    continue
                if lock_addr == addr and spinning.GetValueAsUnsigned() != 0:
                    cpu = hal_utils.get_current_cpu(frame)
                    cpu_addr = hal_utils.get_address_of_value(cpu)
                    print(f'Core {i + 1}: {hex(cpu_addr)}')

class DumpRwSpinlockCommand(ParsedCommand):
    def setup_command_definition(self):
        parser = self.get_parser()
        args = [parser.make_argument_element(lldb.eArgTypeAddress, repeat='plain')]
        parser.add_argument_set(args)
        parser.add_option(short_option='v',
                          long_option='verbose',
                          help='Verbose output',
                          default=False,
                          value_type=lldb.eArgTypeBoolean)

    def get_short_help(self):
        return "Dump rwspinlock based on address."

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused

    def _in_lock_acquire(self, func_name):
        if func_name == 'RwSpinlockAcquire':
            return True

        if func_name == 'RecRwSpinlockAcquire':
            return True
        
        return False

    def __call__(self, debugger, args_array, exe_ctx, result):
        buff = ctypes.create_string_buffer(b'\0' * 256)
        addr = args_array.GetItemAtIndex(0).GetStringValue(ctypes.addressof(buff))

        try:
            addr = int(addr, base=16)
        except ValueError:
            print('Invalid hexadecimal address!')
            return
        
        lock = hal_utils.get_value_from_address(hal_utils.RW_SPINLOCK_TYPE, addr)
        print(f'Lock {hex(addr)}:')
        if self.get_parser().verbose:
            print(lock)
        print('Waiters:')
        
        core_count = hal_utils.DEBUGGER_PROCESS.GetNumThreads()
        for i in range(core_count):
            core = hal_utils.DEBUGGER_PROCESS.GetThreadAtIndex(i)
            frame = core.GetSelectedFrame()
            if self._in_lock_acquire(frame.GetDisplayFunctionName()):
                spinning = frame.FindVariable('spinning')
                if not spinning:
                    continue
                lock_addr = frame.FindVariable('Spinlock').GetValueAsAddress()
                if not lock_addr:
                    continue
                if lock_addr == addr and spinning.GetValueAsUnsigned() != 0:
                    cpu = hal_utils.get_current_cpu(frame)
                    cpu_addr = hal_utils.get_address_of_value(cpu)
                    print(f'Core {i + 1}: {hex(cpu_addr)}')
