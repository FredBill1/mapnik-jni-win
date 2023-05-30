package mapnik.outputformat;

import java.util.HashMap;
import java.util.Map;

public abstract class FormatStringBuilderBase {

    protected final Map<String, String> options;
    private String formatString = null;

    protected FormatStringBuilderBase() {
        options = new HashMap();
    }

    protected FormatStringBuilderBase(Map<String, String> options) {
        this.options = options;
    }

    protected abstract String getFormatName();

    protected Map<String, String> addOption(String key, String value) {
        Map<String, String> optionsCopy = new HashMap<>(this.options);
        optionsCopy.put(key, value);
        return optionsCopy;
    }

    @Override
    public String toString() {
        if (formatString == null) {
            formatString = buildFormatString();
        }
        return formatString;
    }

    private String buildFormatString() {
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append(getFormatName());
        for (Map.Entry<String, String> entry : options.entrySet()) {
            stringBuilder.append(':');
            stringBuilder.append(entry.getKey());
            stringBuilder.append('=');
            stringBuilder.append(entry.getValue());
        }
        return stringBuilder.toString();
    }
}
