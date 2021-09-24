import com.zenvv.capi.*;

public class TestJni {
    public static void main(String[] args) {
        byte[] data = new byte[960];
        AEncoder enc = new AEncoder(AEncoder.OPUS_CODEC);
        byte[] ret1 = enc.Encode(data);
        int err1 = enc.GetLastError();
        enc.Close();
        int len1 = (ret1 != null) ? ret1.length : 0;
        System.out.println("encode:" + len1 + ", err:" + err1);

        ADecoder dec = new ADecoder(ADecoder.OPUS_CODEC);
        byte[] ret2 = dec.Decode(ret1);
        int err2 = dec.GetLastError();
        dec.Close();
        int len2 = (ret2 != null) ? ret2.length : 0;
        System.out.println("decode:" + len2 + ", err:" + err2);
    }
}
