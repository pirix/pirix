pub fn init() {

}

pub unsafe fn kernel_map<T>(addr: *mut T) -> *mut T {
    return 0 as *mut T;
}

pub unsafe fn kernel_unmap<T>(addr: *mut T) {

}
