import lldb
from lldb.plugins.parsed_cmd import ParsedCommand

import hal_utils

_notifications = {}

def register_notification(function_name, callback):
    if function_name in _notifications:
        print(f'Notification {function_name} already registered!')
        return

    _notifications[function_name] = (callback, False)
    print(f'Notification {function_name} registered.')

def receive_notification(frame, bp_loc, internal_dict):
    global _notifications

    core = frame.GetThread()

    if core.GetNumFrames() <= 1:
        hal_utils.DEBUGGER_PROCESS.Continue()

    notifier_frame = core.GetFrameAtIndex(1)
    function_name = notifier_frame.GetFunctionName()

    # print(f'Notification received!')
    # print(f'Notifier Frame: {notifier_frame}')
    # print(f'Notifier Frame rip: {hex(notifier_frame.GetPC())}')

    for name, (callback, tracked) in _notifications.items():
        if function_name == name:
            # print(f'Handling notification {function_name}')
            if tracked: 
                loc = None
                if core.GetNumFrames() >= 3:
                    loc = core.GetFrameAtIndex(2)
                if loc:
                    print(f'Event {name} at {loc}') 
            callback(core, notifier_frame) 
            break

    hal_utils.DEBUGGER_PROCESS.Continue()

class TrackCommand(ParsedCommand):

    def setup_command_definition(self):
        None

    def get_short_help(self):
        return 'Track an already registered notification.'        

    def get_long_help(self):
        return 'Usage: <function_name> <on|off>'

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused
    
    def __call__(self, debugger, args_array, exe_cxt, result):
        global _notifications

        args = args_array.split()
        if len(args) < 2:
            print('Insufficient arguments.')
            return
        name = args[0]
        tracked = args[1]

        if tracked == 'on':
            tracked = True
        elif tracked == 'off':
            tracked = False
        else:
            print('Invalid toggle argument.')
            return

        if not name in _notifications:
            print('Notification not found.')
            return
        
        cbk, _ = _notifications[name]
        _notifications[name] = (cbk, tracked)
