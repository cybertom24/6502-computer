package romImage;

import java.util.stream.Collectors;
import java.util.stream.Stream;

final class Option {
    private String path;
    private String output;
    private long start;

    public Option(String path, String output, long start) {
        this.path = path;
        this.output = output;
        this.start = start;
    }

    public void overrideUndefined(Option overrider) {
        if(path == null)
            path = overrider.getPath();
        if(output == null)
            output = overrider.getOutput();
        if(start < 0)
            start = overrider.getStart();
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }

    public String getOutput() {
        return output;
    }

    public void setOutput(String output) {
        this.output = output;
    }

    public long getStart() {
        return start;
    }

    public void setStart(long start) {
        this.start = start;
    }

    public static Option parseArguments(String[] args) {
        String path = getParameter(args, "-path=");
        String output = getParameter(args, "-out=");

        long start = -1;
        String startString = getParameter(args, "-start=");
        if(startString != null)
            start = Long.parseLong(startString);

        return new Option(path, output, start);
    }

    private static String getParameter(String[] array, String parameterStart) {
        String result = null;

        try {
            String res = (String) Stream.of(array).filter(str -> str.startsWith(parameterStart))
                    .collect(Collectors.toSet()).toArray()[0];

            result = res.split(parameterStart)[1];
        } catch (IndexOutOfBoundsException e) {
            System.out.println("> No \"" + parameterStart + "\" argument found");
            //e.printStackTrace();
        }

        return result;
    }

    private static boolean isParameterPresent(String[] array, String parameter) {
        // If the parameter is present the size will be greater than 0
        return Stream.of(array).filter(str -> str.startsWith(parameter))
                .collect(Collectors.toSet()).size() > 0;
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder("Options:\n-path = ");

        if (path == null)
            builder.append("undefined");
        else
            builder.append(path);

        builder.append("\n-out = ");
        if (output == null)
            builder.append("undefined");
        else
            builder.append(output);

        builder.append("\n-start = ");
        if (start < 0)
            builder.append("undefined");
        else
            builder.append(start);

        return builder.toString();
    }
}
