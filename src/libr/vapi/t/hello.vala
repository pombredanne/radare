using GLib;
using Radare; //.Hash;

public class HashExample
{
	private static void printChecksum(string str, uint8 *buf, int size)
	{
		stdout.printf(str);
		for(int i=0;i<size; i++)
			stdout.printf("%02x", buf[i]);
		stdout.printf("\n");
	}

	public static void main(string[] args)
	{
		/* calculate crc32 */
		stdout.printf("CRC32: %x\n", Hash.crc32("hello", 5));

		/* calculate md5 */
		Hash.State st = new Hash.State();
		uint8 *md = st.md5("hello", 5);
		printChecksum("MD5: ", md, Hash.Size.MD5);
	}
}
