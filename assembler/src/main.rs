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

// const RXRX:Vec<String> = vec!["mov".to_string(), "not", "and", "or", "lw", "sw"];
static RXRX:[(&str, u16); 6] = [("mov", 0x01), ("not", 0x08), ("and", 0x09), ("or", 0x0a), ("lw", 0x0e), ("sw", 0x0f)];


// imm = hexadecimal | bas10
// rX = r \[0-7\]

fn encode_instruction(line: &String) -> u16 {
    //tar in en rad
    //ex, mov r5 r3
    //transform into 16 bitar

    // so like & -> https://www.w3schools.com/rust/rust_borrowing_references.php
    
    let mut results: u16 = 0x0;

    let mnemonic: Vec<String> = line.split(" ").map(|x| String::from(x)).collect(); //https://en.wikipedia.org/wiki/Mnemonic
    let action: &String = &mnemonic[0];
    
    // if RXRX.iter().find(&action[0].as_str()) { }


    // if RXRX.contains(action.as_str()) {
    for i in 0..RXRX.len() {
        if RXRX[i].0 == action.as_str() {
            let from = mnemonic[1].chars().nth(1).unwrap().to_digit(10).unwrap() as u16;
            let to = mnemonic[2].chars().nth(1).unwrap().to_digit(10).unwrap() as u16;
            
            // find the hexadecimal cororesponding the word
            results = RXRX[i].1 << 11;

            // format!("{:b}", from); //formats to binary
            results = results | (from << 3);
            results = results | to;
            return results;
        }
    }
    if action == "nop"{
        return results
    } else if action == "movl" {
        //movl - move lower -> most last part
        
        //pasring imm
        let imm_str = mnemonic[1].trim();
        let imm = if imm_str.starts_with("0x") || imm_str.starts_with("0X") {
            //its hexadecimal
            u16::from_str_radix(&imm_str[2..], 16).unwrap()
            //from_str_radix = translates from radix to int
        } else {
            u16::from_str_radix(&imm_str[2..], 10).unwrap()
        };
        
        let to = mnemonic[2].chars().nth(1).unwrap().to_digit(10).unwrap() as u16;

        results = (0x02 << 11) | ((imm & 0b11111111) << 3) | (to);
        return results
    } else if action == "b" || action == "bz" {
        let imm_str = mnemonic[1].trim();
        let imm = if imm_str.starts_with("0x") || imm_str.starts_with("0X") {
            //its hexadecimal
            u16::from_str_radix(&imm_str[2..], 16).unwrap()
            //from_str_radix = translates from radix to int
        } else {
            u16::from_str_radix(&imm_str, 10).unwrap()
        };

        results = (if action == "b" {0x10 << 11} else {0x14 << 11}) | imm;
        return results;
    } else if action == "halt" {
        results = 0x1f << 11;
        return results;
    } else if action.starts_with("add") || action.starts_with("sub") {
        results = if action.starts_with("add") {0x04 << 11} else {0x05 << 11};
        let s_or_no = if action.ends_with("s") {1} else {0};
        
        let from = mnemonic[1].chars().nth(1).unwrap().to_digit(10).unwrap() as u16;
        let to = mnemonic[2].chars().nth(1).unwrap().to_digit(10).unwrap() as u16;
        results = results | s_or_no <<10 | from << 3 | to;
        return results
    } else {
        panic!("thats not one of the instructions")
    }
}


fn main() {
    let ipath: String = std::env::args().nth(1).expect("input path given"); //args =  cargo run -- path
    let opath: String = std::env::args().nth(2).expect("output path given");
    let mut data: [u8; 0x10000] = [0; 0x10000]; //this represents virtrual memory
    let ifile = File::open(ipath).unwrap(); 
    let reader = BufReader::new(ifile);
    let mut current_pos: usize = 0; 
    let mut line_number: u16 = 1; //keep count. know which line failed.
    for line_res in reader.lines() {
        if line_res.is_err() {
            break;
        }
        let line: String = line_res.unwrap(); 

        let line:String = line.trim().to_string();
        if line.is_empty() { continue; }


        if line.chars().nth(0).unwrap().is_ascii_lowercase() { //i
            let encoded_value = encode_instruction(&line);
            data[current_pos] = encoded_value as u8;
            data[current_pos+1] = (encoded_value >> 8) as u8;
            current_pos += 2;
        } else {
        match line.chars().nth(0).unwrap() {
            ';' => {
                continue;
            },
            '@' => {
                if let Some(rest) = line.strip_prefix("@align") { //strips the first thing. so smidigt.
                    //strip_prefix() gets rid of the word
                    let n: usize = rest.trim().parse().unwrap();//.to_string().chars().to_digit(10); // Removes leading/trailing whitespace char is only one character
                    // current_pos = -1*(-1*current_pos & ((1<<current_pos)-1));
                    // current_pos = current_pos & (current_pos - 1) 
                    current_pos = (current_pos + n - 1) & !(n - 1);
                    // println!("The value of n is: {}", n);
                }
                if let Some(rest) = line.strip_prefix("@org") {
                    let n: usize = rest.trim().parse().unwrap();
                    current_pos = n;
                }
            },
            '%' => {
                if let Some(rest) = line.strip_prefix("%db") {
                    //alltid en byte
                    let n :u8 = rest.trim().parse().unwrap();
                    data[current_pos] = n;
                    current_pos += 1

                }
                if let Some(rest) = line.strip_prefix("%dw") {
                    // alltid två bytes. en sträng = 2 bytes
                    let n: u16 = rest.trim().parse().unwrap();
                    let first:u8 = (0b11111111 & n) as u8;//.try_into().unwrap();
                    // let last: u8 = (0b11111111 & n).try_into().unwrap();
                    let last:u8 = (0b11111111 & (n >> 8)) as u8;
                    data[current_pos] = first; 
                    data[current_pos+1] = last;
                    current_pos += 2

                }
                if let Some(rest) = line.strip_prefix("%fb") {
                    // let n = rest[0].trim().parse().unwrap(); //unknown amount of bytes. v n
                    if let Some((value_wrap, amount_wrap)) = rest.split_once(",") {
                        let value:u8 = value_wrap.trim().parse().unwrap();
                        let amount = amount_wrap.trim().parse().unwrap();

                        for _ in 0..amount {
                            data[current_pos] = value; //(0b11111111 & value) as u8;
                            current_pos += 1;
                            // value = (n >> 8)
                        }
                    }
                }
                if let Some(rest) = line.strip_prefix("%fw") {
                    if let Some((value_wrap, amount_wrap)) = rest.split_once(",") {
                        let value:u16 = value_wrap.trim().parse().unwrap();
                        let first: u8 = (0b11111111 & value) as u8;
                        let last: u8 = (0b11111111 & (value >> 8)) as u8;
                        let amount = amount_wrap.trim().parse().unwrap();

                        for _ in 0..amount {
                            data[current_pos] = first;
                            data[current_pos+1] = last;
                            current_pos += 2
                        }
                    }
                }
            },
            _ => { panic!("Invalid line start on line {line_number}"); },
        }
        }
        line_number += 1;
    }
    std::fs::write(opath, &data[0..current_pos]).expect("Failed to write output file");
    println!("Hello, world!");
}


// db - define bytes -> u16 cause byte = u8
// dw - define word -> 16 bytes too?

//making test cases

#[test]
fn encode_halt() {
    let line = "halt".to_string();
    assert_eq!(encode_instruction(&line), 0x1f<<11)
}

#[test]
fn encode_b_imm() {
    let line = "b 0x01".to_string();
    assert_eq!(encode_instruction(&line), 0x10<<11 | 0x01)
}
#[test]
fn encode_bz_imm2() {
    let line = "bz 7".to_string();
    assert_eq!(encode_instruction(&line), 0x14<<11 | 7)
}

#[test]
fn encode_sw() {
    let line = "sw r5 r4".to_string();
    assert_eq!(encode_instruction(&line), 0x0f<<11 | 0b101 <<3 | 0b100)
}

#[test]
fn encode_movl() { 
    let line = "movl 0x0f r4".to_string();
    assert_eq!(encode_instruction(&line), 0x02<<11 | 0x0f <<3 | 4)
}
