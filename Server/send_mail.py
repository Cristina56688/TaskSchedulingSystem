import smtplib
import sys
from email.mime.text import MIMEText
from email.header import Header

def send_mail(to_email, subject, body, sender_email, sender_password):
    try:
        msg = MIMEText(body, 'plain', 'utf-8')
        msg['Subject'] = Header(subject, 'utf-8')
        msg['From'] = sender_email
        msg['To'] = to_email

        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()
        server.login(sender_email, sender_password)
        server.sendmail(sender_email, [to_email], msg.as_string())
        server.quit()
        return True
    except Exception as e:
        print(f"Error: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 6:
        print("Usage: python3 send_mail.py <to> <subject> <body> <sender_email> <sender_password>")
        sys.exit(1)
    
    to_email = sys.argv[1]
    subject = sys.argv[2]
    body = sys.argv[3]
    sender_email = sys.argv[4]
    sender_password = sys.argv[5]
    
    if send_mail(to_email, subject, body, sender_email, sender_password):
        sys.exit(0)
    else:
        sys.exit(1)
