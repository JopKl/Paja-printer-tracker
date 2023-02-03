use printer::Printer;

struct Server{
    pub name: String,
    printer_list: Vec,
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

impl Server{
    pub fn new(name:String)->Self{
        Server{   
            name,
            printer_list: Vec<&Printer>::new(),
        }
    }

    // Read a text file and create Printers based on name and ip
    pub fn read_file(&self, filepath: String)->Result<(), std::io::Error>{
        if let Ok(lines) = read_lines(filepath){
            for line in lines{
                if let Ok(info) = String::from(line){
                    let info_vec: Vec = info.split(';').collect();
                    if info_vec.len() != 2{
                        continue;
                    }
                    self.printer_list.push(Printer::new(info_vec[0],
                        info_vec[1]));
                }
            }
        }
    }

    // Update printers status
    pub fn update_status(&self){}

    // Send data to REST API
    pub fn send_data(self){}

    // Update database
    pub fn update_database(self){}
}