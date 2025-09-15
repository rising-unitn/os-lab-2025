#!/bin/bash
# An interactive menu system for system administration tasks

function show_menu {
    clear
    echo "=== System Admin Menu ==="
    echo "1. Show disk usage"
    echo "2. List top 5 CPU-consuming processes"
    echo "3. Check system uptime"
    echo "4. Back up home directory"
    echo "5. Exit"
    echo "========================="
}

function disk_usage {
    echo "Disk Usage:"
    df -h | grep -v "tmpfs" | grep -v "devtmpfs"
    echo
    read -p "Press Enter to continue..."
}

function top_processes {
    echo "Top 5 CPU-consuming processes:"
    ps aux --sort=-%cpu | head -6
    echo
    read -p "Press Enter to continue..."
}

function check_uptime {
    echo "System uptime:"
    uptime
    echo
    read -p "Press Enter to continue..."
}

function backup_home {
    echo "Backing up home directory..."
    timestamp=$(date +%Y%m%d_%H%M%S)
    tar -czf "home_backup_$timestamp.tar.gz" $HOME 2>/dev/null
    
    if [ $? -eq 0 ]; then
        echo "Backup created: home_backup_$timestamp.tar.gz"
    else
        echo "Error creating backup!"
    fi
    
    echo
    read -p "Press Enter to continue..."
}

# Main loop
while true; do
    show_menu
    read -p "Enter your choice [1-5]: " choice
    
    case $choice in
        1) disk_usage ;;
        2) top_processes ;;
        3) check_uptime ;;
        4) backup_home ;;
        5) echo "Goodbye!"; exit 0 ;;
        *) echo "Invalid option. Press Enter to continue..."; read ;;
    esac
done