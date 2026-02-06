import lldb
from lldb.plugins.parsed_cmd import ParsedCommand

import hal_threads
import hal_processes
import hal_notify
import hal_utils
import hal_files
import hal_cpus
import hal_vmm
import hal_sync
from hal_utils import HalCommandType
from hal_utils import HalCommand

def __lldb_init_module(debugger, internal_dict):
    print('Loading HAL9000 module...')

    print('Initializing hal_utils')
    hal_utils.init(debugger)

    commands = [
        HalCommand('hal_dbg.hal_utils.hal_commands', 
                   HalCommandType.HalCommandTypeFunction, 
                   'hal_commands'),
        HalCommand('hal_dbg.hal_threads.ListThreadsCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'list_threads'),
        HalCommand('hal_dbg.hal_threads.DumpThreadCommand',
                   HalCommandType.HalCommandTypeClass,
                   'dump_thread'),
        HalCommand('hal_dbg.hal_processes.ListProcessesCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'list_processes'),
        HalCommand('hal_dbg.hal_processes.DumpProcessCommand',
                   HalCommandType.HalCommandTypeClass,
                   'dump_process'),
        HalCommand('hal_dbg.hal_threads.ListRunningThreadsCommand',
                   HalCommandType.HalCommandTypeClass,
                   'list_running_threads'),
        HalCommand('hal_dbg.hal_files.ListFileObjectsCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'list_file_objects'),
        HalCommand('hal_dbg.hal_files.DumpFileObjectCommand',
                   HalCommandType.HalCommandTypeClass,
                   'dump_file_object'),
        HalCommand('hal_dbg.hal_vmm.DumpVmmReservationSpaceCommand',
                   HalCommandType.HalCommandTypeClass,
                   'dump_vmm_reservation_space'),
        HalCommand('hal_dbg.hal_vmm.DumpVmmReservationCommand',
                   HalCommandType.HalCommandTypeClass,
                   'dump_vmm_reservation'),
        HalCommand('hal_dbg.hal_sync.ListLocksCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'list_locks'),
        HalCommand('hal_dbg.hal_sync.DumpSpinlockCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'dump_spinlock'),
        HalCommand('hal_dbg.hal_sync.DumpRwSpinlockCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'dump_rwspinlock'),
        HalCommand('hal_dbg.hal_sync.ListMutexesCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'list_mutexes'),
        HalCommand('hal_dbg.hal_sync.DumpMutexCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'dump_mutex'),
        HalCommand('hal_dbg.hal_sync.ListExEventsCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'list_ex_events'),
        HalCommand('hal_dbg.hal_sync.DumpExEventCommand',
                   HalCommandType.HalCommandTypeParsed,
                   'dump_ex_event'),
        HalCommand('hal_dbg.hal_cpus.ListCpusCommand',
                   HalCommandType.HalCommandTypeClass,
                   'list_cpus'),
        HalCommand('hal_dbg.hal_cpus.DumpCpuCommand',
                   HalCommandType.HalCommandTypeClass,
                   'dump_cpu')
    ]

    hal_utils.add_commands(debugger, commands)

    print('Loaded HAL9000 module.')
