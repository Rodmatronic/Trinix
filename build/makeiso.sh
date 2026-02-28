#!/bin/sh

set -e

ISO_NAME="triunix.iso"
WORKDIR="isotree"
KERNEL="kernel/triunix"
FILE="/triunix"

rm -rf "$WORKDIR" "$ISO_NAME"
mkdir -p "$WORKDIR/boot/grub"

cat > "$WORKDIR/boot/grub/grub.cfg" <<EOF
set timeout=5

menuentry "Trinix at $FILE" {
	echo -n 'Loading...'
	multiboot2 $FILE
	echo 'Done.'
	boot
}
EOF

cp $KERNEL "$WORKDIR/triunix"
grub-mkrescue -o "$ISO_NAME" "$WORKDIR" \
  --compress=xz \
  --fonts= \
  --locales= \
  --themes= \
  --install-modules="normal iso9660 echo multiboot2" \
  --modules="normal iso9660 echo multiboot2"

echo "ISO created: $ISO_NAME"
