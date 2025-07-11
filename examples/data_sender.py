#!/usr/bin/env python3
"""
ESP32 System Monitor - Data Sender
Sends real-time system monitoring data to ESP32 display via UDP

Requirements:
    pip install psutil

Usage:
    python data_sender.py [ESP32_IP]
    
Example:
    python data_sender.py 192.168.1.100
"""

import socket
import json
import psutil
import time
import sys
import platform
from datetime import datetime

class SystemMonitor:
    def __init__(self, esp32_ip="192.168.1.100", port=12345):
        self.esp32_ip = esp32_ip
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        
        print(f"ESP32 System Monitor Data Sender")
        print(f"Target: {esp32_ip}:{port}")
        print(f"Platform: {platform.system()} {platform.release()}")
        print("-" * 50)
    
    def get_temperature(self):
        """Get CPU temperature (platform dependent)"""
        try:
            if platform.system() == "Linux":
                # Try common Linux temperature sensors
                temps = psutil.sensors_temperatures()
                if 'coretemp' in temps:
                    return int(temps['coretemp'][0].current)
                elif 'acpi' in temps:
                    return int(temps['acpi'][0].current)
                elif 'thermal_zone0' in temps:
                    return int(temps['thermal_zone0'][0].current)
            
            elif platform.system() == "Windows":
                # Windows temperature requires additional libraries
                # For now, simulate temperature based on CPU usage
                cpu_percent = psutil.cpu_percent()
                return int(30 + (cpu_percent * 0.4))  # Simulate 30-70°C range
            
            elif platform.system() == "Darwin":  # macOS
                # macOS temperature requires additional tools
                # Simulate based on CPU usage
                cpu_percent = psutil.cpu_percent()
                return int(35 + (cpu_percent * 0.3))  # Simulate 35-65°C range
                
        except Exception as e:
            print(f"Temperature reading error: {e}")
        
        # Fallback: simulate temperature
        return int(40 + (psutil.cpu_percent() * 0.2))
    
    def get_network_speed(self):
        """Calculate network speed in MB/s"""
        try:
            # Simple network activity indicator
            net_io = psutil.net_io_counters()
            # This is cumulative, for real speed calculation you'd need to 
            # store previous values and calculate difference
            total_mb = (net_io.bytes_sent + net_io.bytes_recv) / 1024 / 1024
            return round(total_mb % 100, 1)  # Mod 100 to keep it reasonable for display
        except:
            return 0.0
    
    def get_volume_level(self):
        """Get system volume level (platform dependent)"""
        try:
            if platform.system() == "Windows":
                # Windows volume requires additional libraries
                # For now, return a simulated value
                return 75
            else:
                # Linux/macOS volume detection would require additional setup
                return 50
        except:
            return 0
    
    def collect_system_data(self):
        """Collect all system monitoring data"""
        try:
            # Get system stats
            cpu_percent = psutil.cpu_percent(interval=0.1)
            memory = psutil.virtual_memory()
            disk = psutil.disk_usage('/')
            
            data = {
                "cpu": round(cpu_percent, 1),
                "ram": round(memory.percent, 1),
                "disk": round(disk.percent, 1),
                "temp": self.get_temperature(),
                "network": self.get_network_speed(),
                "volume": self.get_volume_level(),
                "time": datetime.now().strftime("%H:%M:%S")
            }
            
            return data
            
        except Exception as e:
            print(f"Error collecting system data: {e}")
            return None
    
    def send_data(self, data):
        """Send data to ESP32 via UDP"""
        try:
            json_data = json.dumps(data)
            self.sock.sendto(json_data.encode('utf-8'), (self.esp32_ip, self.port))
            return True
        except Exception as e:
            print(f"Error sending data: {e}")
            return False
    
    def run(self):
        """Main monitoring loop"""
        print("Starting system monitoring... (Press Ctrl+C to stop)")
        
        try:
            while True:
                # Collect system data
                data = self.collect_system_data()
                
                if data:
                    # Send to ESP32
                    if self.send_data(data):
                        # Print status
                        print(f"[{data['time']}] CPU:{data['cpu']:5.1f}% "
                              f"RAM:{data['ram']:5.1f}% "
                              f"DISK:{data['disk']:5.1f}% "
                              f"TEMP:{data['temp']:2d}°C "
                              f"NET:{data['network']:4.1f}MB/s "
                              f"VOL:{data['volume']:2d}%")
                    else:
                        print(f"[{data['time']}] Failed to send data")
                
                # Wait before next update
                time.sleep(1)
                
        except KeyboardInterrupt:
            print("\nStopping system monitor...")
        except Exception as e:
            print(f"Unexpected error: {e}")
        finally:
            self.sock.close()

def main():
    # Get ESP32 IP from command line argument or use default
    esp32_ip = sys.argv[1] if len(sys.argv) > 1 else "192.168.1.100"
    
    # Validate IP format (basic check)
    try:
        socket.inet_aton(esp32_ip)
    except socket.error:
        print(f"Error: Invalid IP address '{esp32_ip}'")
        print("Usage: python data_sender.py [ESP32_IP]")
        sys.exit(1)
    
    # Create and run monitor
    monitor = SystemMonitor(esp32_ip)
    monitor.run()

if __name__ == "__main__":
    main()
