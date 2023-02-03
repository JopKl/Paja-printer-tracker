#![warn(clippy::all)]
// mod server;
mod printer;
use printer::Printer;
fn main() {
    let mut um5 = Printer::new("UM5".to_string(),
        "http://{}/api/v1".to_string());
    um5.get_data_um().unwrap();
    um5.print_data();
}