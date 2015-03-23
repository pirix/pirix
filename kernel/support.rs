extern "rust-intrinsic" {
    fn offset<T>(dst: *const T, offset: isize) -> *const T;
}

#[no_mangle]
pub unsafe extern "C" fn memcpy(dest: *mut u8, src: *const u8,
                                n: usize) -> *mut u8 {
    let mut i = 0;
    while i < n {
        *(offset(dest as *const u8, i as isize) as *mut u8) =
            *offset(src, i as isize);
        i += 1;
    }
    return dest;
}

#[no_mangle]
pub unsafe extern "C" fn memmove(dest: *mut u8, src: *const u8,
                                 n: usize) -> *mut u8 {
    if src < dest as *const u8 { // copy from end
        let mut i = n;
        while i != 0 {
            i -= 1;
            *(offset(dest as *const u8, i as isize) as *mut u8) =
                *offset(src, i as isize);
        }
    } else { // copy from beginning
        let mut i = 0;
        while i < n {
            *(offset(dest as *const u8, i as isize) as *mut u8) =
                *offset(src, i as isize);
            i += 1;
        }
    }
    return dest;
}

#[no_mangle]
pub unsafe extern "C" fn memset(s: *mut u8, c: i32, n: usize) -> *mut u8 {
    let mut i = 0;
    while i < n {
        *(offset(s as *const u8, i as isize) as *mut u8) = c as u8;
        i += 1;
    }
    return s;
}
