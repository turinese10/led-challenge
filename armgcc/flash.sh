#!/bin/sh
arm-none-eabi-objcopy -O binary debug/led_challenge.elf debug/led_challenge.bin

DEVICE="/dev/sda"

# Detect existing mount point
MOUNT_POINT=$(lsblk -no MOUNTPOINT "$DEVICE" | head -n 1)

if [ -z "$MOUNT_POINT" ]; then
    echo "Device not mounted, mounting..."
    MOUNT_OUTPUT=$(udisksctl mount -b "$DEVICE" 2>&1)
    echo "$MOUNT_OUTPUT"

    # Extract new mount point from udisksctl output
    MOUNT_POINT=$(echo "$MOUNT_OUTPUT" | grep -oP 'Mounted .* at \K.*')
fi

# If still empty, fail
if [ -z "$MOUNT_POINT" ]; then
    echo "ERROR: Could not find mount point for $DEVICE."
    exit 1
fi

echo "Using mount point: $MOUNT_POINT"

# Flash
cp debug/led_challenge.bin "$MOUNT_POINT/"
echo "Flashed to $MOUNT_POINT"
