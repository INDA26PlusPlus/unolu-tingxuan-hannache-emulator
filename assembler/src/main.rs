// Align directive simply forwards the curred data index
// Origin simply sets the current data index
// Error if you try to overwrite to non-zero data
/*
instruction
; comment
@directive 
%datadirective

@align n
set current_pos to -1*(-1*current_pos & ((1<<current_pos)-1))

@org a
set current_pos to a

%db v
Define byte with value v

%dw v
Define word with value v

%fb v, n
Fill with n bytes with value v

%fw v, n
Fill with n words with value v



*/

use std::fs::File;
use std::io::{prelude::*, BufReader};


fn encode_instruction(line: &String) -> u16 {
    
}


fn main() {
    let ipath: String = std::env::args().nth(1).expect("input path given");
    let opath: String = std::env::args().nth(2).expect("output path given");
    let mut data: [u8; 0x10000] = [0; 0x10000];
    let ifile = File::open(ipath).unwrap();
    let reader = BufReader::new(ifile);
    let mut current_pos: usize = 0; 
    let mut line_number: u16 = 1;
    for line_res in reader.lines() {
        if line_res.is_err() {
            break;
        }
        let line: String = line_res.unwrap();
        if line.chars().nth(0).unwrap().is_ascii_lowercase() {
            data[current_pos] = encode_instruction(&line) as u8;
            data[current_pos] = (encode_instruction(&line) >> 8) as u8;
            current_pos += 2;
        }
        match line.chars().nth(0).unwrap() {
            ';' => {
                continue;
            },
            '@' => {
            },
            '%' => {
            },
            _ => { panic!("Invalid line start on line {line_number}"); },
        }
        line_number += 1;
    }
    println!("Hello, world!");
}
