package romImage;

import java.io.*;

public class ROMImageMain {

    private static final long EEPROM_LENGTH = 0x8000;
    private static final long EEPROM_START_ADDRESS = 0x8000;
    private static final long ABSOLUTE_LOW_BYTE_START_VECTOR = 0xFFFC;
    private static final long ABSOLUTE_UP_BYTE_START_VECTOR = 0xFFFD;
    private static final long RELATIVE_LOW_BYTE_START_VECTOR = ABSOLUTE_LOW_BYTE_START_VECTOR - EEPROM_START_ADDRESS;
    private static final long RELATIVE_UP_BYTE_START_VECTOR = ABSOLUTE_UP_BYTE_START_VECTOR - EEPROM_START_ADDRESS;

    private static final Option OPTION_DEFAULT = new Option(null, "out.bin", 0);

    private static final byte NOP_OPCODE = (byte) 0xEA;
    private static final long LOW_BYTE_MASK = 0x00FF;
    private static final long UP_BYTE_MASK = 0xFF00;


    public static void main(String[] args) throws IOException {
        String[] prova = {"-path=code.65b", "-start=64000", "-out=image.bin"};

        Option option = Option.parseArguments(args);
        option.overrideUndefined(OPTION_DEFAULT);
        System.out.println(option);

        if(option.getPath() == null) {
            System.err.println("No input file has been specified");
            abort();
        }

        File input = new File(option.getPath());
        if(!input.canRead()) {
            System.err.println("Cannot read file: " + option.getPath());
            abort();
        }

        // Check if the input file is long enough
        if(input.length() < option.getStart()) {
            System.err.println("The input file is not long enough! Expected: " + option.getStart() + ", found: " + input.length());
            abort();
        }

        File output = new File(option.getOutput());

        InputStream inStream = new BufferedInputStream(new FileInputStream(input));
        OutputStream outStream = new BufferedOutputStream(new FileOutputStream(output));

        System.out.println("> Reading file...");

        byte[] inputBuffer = inStream.readAllBytes();
        inStream.close();

        System.out.println("> File read");
        System.out.println("> Writing on file...");

        for(long i = 0; i < EEPROM_LENGTH; i++) {
            if(i == RELATIVE_LOW_BYTE_START_VECTOR)
                outStream.write((byte) (option.getStart() & LOW_BYTE_MASK));
            else if(i == RELATIVE_UP_BYTE_START_VECTOR)
                outStream.write((byte) ((option.getStart() & UP_BYTE_MASK) >> 8));
            else if(i >= inputBuffer.length - option.getStart())
                outStream.write(NOP_OPCODE);
            else
                outStream.write(inputBuffer[(int) (i + option.getStart())]);
        }
        outStream.close();

        System.out.println("> File written");
        System.out.println("> Operation completed");
        System.exit(0);
    }

    private static void abort() {
        System.err.println("Operation aborted");
        System.exit(-1);
    }
}
