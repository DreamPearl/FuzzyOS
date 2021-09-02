#include <fuzzy/kernel/process/process.h>
#include <fuzzy/kernel/panic.h>

int process_scheduler_rr(int lastpid) {
    // Round Robin Scheduler.
    int best_id = -1;  // not found
    for (int i = 1; i <= MAX_PROCESS; ++i) {
        int nid = (lastpid+i)%MAX_PROCESS;
        struct Process *process = get_process(nid);
        if (process->state == STATE_READY) {
            best_id = nid;
            break;
        }
    }
    return best_id;
}

int process_scheduler_largestpid(int lastpid) {
    // Run largest READY pid
    int best_id = -1;  // not found
    for (int nid = MAX_PROCESS-1; nid >= 0; --nid) {
        struct Process *process = get_process(nid);

        if (process->state == STATE_READY) {
            best_id = nid;
            break;
        }
    }
    return best_id;
}

void process_scheduler_stash_state() {
    for (int id = 0; id < MAX_PROCESS; id++) {
        struct Process *process = get_process(id);

        if(process->state == STATE_EXIT) {
            // process unallocate ready to be killed
            process->state = STATE_COLD;
        } else if(process->state == STATE_RUNNING) {
            // process move running process to ready
            process->state = STATE_READY;
        }
    }
}

int process_scheduler_get_next_pid(int lastpid) {
    process_scheduler_stash_state();
    // execute one of the scheduling algorithm
    return process_scheduler_largestpid(lastpid);
}

void process_scheduler(int *_e_ip, int *_e_cs, int *_e_sp, int *_e_ss) {
    int e_ip = *_e_ip;
    int e_cs = *_e_cs;
    int e_sp = *_e_sp;
    int e_ss = *_e_ss;
    print_info("[process_scheduler] cs:ip %x:%x,  ss:sp %x:%x",
        e_cs, e_ip,
        e_ss, e_sp);

    int pid = get_idt_reverse_pid_lookup_cs(e_cs);
    int npid = process_scheduler_get_next_pid(pid);

    if(npid<0) {
        PANIC(0, "[process_scheduler] no STATE_READY process alive");
    }

    print_info("[process_scheduler] pid: %d -> %d", pid, npid);
    struct Process *process = get_process(pid);
    if(process->state != STATE_COLD) {
        // if last process is still alive
        process->cs = e_cs;
        process->ip = e_ip;
        process->ss = e_ss;
        process->sp = e_sp;
    }
    // last process can be
    //  - RUNNING
    //  - BLOCK  # TODO: implement

    struct Process *nprocess = get_process(npid);
    nprocess->state = STATE_RUNNING;
    e_cs = nprocess->cs;
    e_ip = nprocess->ip;
    e_ss = nprocess->ss;
    e_sp = nprocess->sp;

    *_e_ip = e_ip;
    *_e_cs = e_cs;
    *_e_sp = e_sp;
    *_e_ss = e_ss;
}

int process_waitpid(unsigned int pid, unsigned int blocked_on_pid) {
    // TODO: this implementation has flaws.
    // We need to do better something.
    // It currently allows blocking on any pid, and rely on syscall client
    // for yield.
    struct Process *other_process = get_process(blocked_on_pid);
    if(other_process==NULL) {
        return -1;  // err
    }
    if(other_process->state == STATE_COLD) {
        return 0;  // no error, wait over
    }
    return 1; // no error, keep waiting
}
