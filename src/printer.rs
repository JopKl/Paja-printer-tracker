//! Contains structs and methods for Printers and their data

// pub struct PrinterData{
//     name: String,
//     job_start_date: String,
//     job_end_date: String,
//     last_removal_date: String,

//     job_progress_sec: i32,
//     job_progress_perc: f32,
//     job_total_time: i32,

//     job_status: String,
//     camera_feed: String,
// }
use reqwest::Url;

pub struct Printer{
    pub name: String,
    ip: String,
    // data: PrinterData,

    // Contains the latest print job data
    job_data: String,
}


impl Printer{
    pub fn new(name: String, ip: String) ->Self{
        Printer{
            name,
            ip,
            // data: PrinterData
            job_data: String::new()
        }
    }

    // Get data from Ultimaker printer's web API
    pub fn get_data_um(&mut self) -> Result<(),reqwest::Error>{
        let api_ip = Url::parse(&(self.ip.clone()+"/print_job")).unwrap();
        let mut response = reqwest::get(api_ip)?;
        self.job_data = response.text()?;
        Ok(())
    }

    pub fn print_data(self) {
        print!("{}",self.job_data);
    }

    // Get data from Octoprint server
    // pub fn Get_data_OP() -> Result<(), std::io::Error>{
        
    // }
}

