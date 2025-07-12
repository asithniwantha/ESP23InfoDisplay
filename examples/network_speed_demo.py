#!/usr/bin/env python3
"""
Network Speed Formatting Demo
Demonstrates how the ESP32 will display different network speeds

This shows the automatic unit selection (KB/s vs MB/s) that the ESP32 implements.
"""

def format_network_speed(speed_mbps):
    """
    Simulate the ESP32's network speed formatting logic
    """
    if speed_mbps < 0:
        return "0KB/s"
    
    # Convert from MB/s to KB/s for better granularity
    speed_kbps = speed_mbps * 1024
    
    if speed_kbps < 1024:
        # Less than 1 MB/s, show in KB/s
        if speed_kbps < 10:
            return f"{speed_kbps:.1f}KB/s"
        else:
            return f"{int(speed_kbps)}KB/s"
    else:
        # 1 MB/s or more, show in MB/s
        if speed_mbps < 10:
            return f"{speed_mbps:.1f}MB/s"
        else:
            return f"{int(speed_mbps)}MB/s"

def main():
    print("ESP32 Network Speed Formatting Demo")
    print("=" * 40)
    
    # Test various network speeds
    test_speeds = [
        0.0, 0.001, 0.01, 0.1, 0.25, 0.5, 0.75,
        1.0, 1.5, 2.3, 5.7, 10.2, 25.8, 100.5
    ]
    
    print("Raw Speed (MB/s) -> ESP32 Display")
    print("-" * 40)
    
    for speed in test_speeds:
        formatted = format_network_speed(speed)
        print(f"{speed:>8.3f} MB/s    ->  {formatted:>8}")
    
    print("\nKey Features:")
    print("• Speeds < 1 MB/s: Displayed in KB/s")
    print("• Speeds ≥ 1 MB/s: Displayed in MB/s") 
    print("• Decimal places: 1 for small values, none for large")
    print("• Automatic unit selection for optimal readability")

if __name__ == "__main__":
    main()
