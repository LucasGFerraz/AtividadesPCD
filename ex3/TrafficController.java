public class TrafficController {
	private int bflag = 0;
	private int rflag = 0;
	
	public int get(char name) {
		if (name == 'b') {
			return bflag;
		}
		else return rflag;
	}
	
    public void enterLeft() {
    	while(get('r')!=0 || get('b')!=0) {
    		try {
    			Thread.sleep(30);
    		    } catch (InterruptedException e) {}
    	}
    	synchronized (this) {
            bflag = 1;
        }
    	System.out.println(rflag+" "+bflag);
    }
    public void enterRight() {
    	while(get('b')!=0 || get('r')!=0) {
    		try {
    			Thread.sleep(30);
    		    } catch (InterruptedException e) {}
    	}
    	synchronized (this) {
            rflag = 1;
        }
    	System.out.println(rflag+" "+bflag);
    }
    public void leaveLeft() {
    	synchronized (this) {
            rflag = 0;
        }
    	System.out.println(rflag+" "+bflag);
    }
    public void leaveRight() {
    	synchronized (this) {
            bflag = 0;
        }
    	System.out.println(rflag+" "+bflag);
    }

}